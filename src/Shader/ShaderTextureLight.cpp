// Alex Niemiec 2020

#include "ShaderTextureLight.h"
#include "d3dUtil.h"

ShaderTextureLight::ShaderTextureLight(ID3D11Device* device, int sl_count, int pl_count, float fogStart, float fogRange, Vect fogColor)
	: ShaderBase(device, L"../Assets/Shaders/TextureLight.hlsl"),
	slCount(sl_count),
	plCount(pl_count),
	FogStart(fogStart),
	FogRange(fogRange),
	FogColor(fogColor)
{
	SpotLightData = new SpotLight[slCount];
	PointLightData = new PointLight[plCount];

	// Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElemetns = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElemetns);

	HRESULT hr;

	// camera data buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// light param
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_LightParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightParams);
	assert(SUCCEEDED(hr));

	// world and material data buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WorldMatData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferWorldMatData);
	assert(SUCCEEDED(hr));

	ZeroMemory(&DirLightData, sizeof(DirLightData));
}

ShaderTextureLight::~ShaderTextureLight()
{
	delete[] PointLightData;
	delete[] SpotLightData;

	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
	ReleaseAndDeleteCOMobject(mpBufferLightParams);
	ReleaseAndDeleteCOMobject(mpBufferWorldMatData);
}

void ShaderTextureLight::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices mCamData;
	mCamData.View = view;
	mCamData.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamData, 0, 0);
}

void ShaderTextureLight::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	DirLightData.Light.Ambient = amb;
	DirLightData.Light.Diffuse = dif;
	DirLightData.Light.Specular = sp;
	DirLightData.Direction = dir;
}

void ShaderTextureLight::SetPointLightParameters(int id, const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLight* pPL = &PointLightData[id];
	pPL->Light.Ambient = amb;
	pPL->Light.Diffuse = dif;
	pPL->Light.Specular = sp;
	pPL->Position = pos;
	pPL->Attenuation = att;
	pPL->Range = r;
}

void ShaderTextureLight::SetSpotLightParameters(int id, const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
{
	SpotLight* pSL = &SpotLightData[id];
	pSL->Light.Ambient = amb;
	pSL->Light.Diffuse = dif;
	pSL->Light.Specular = sp;
	pSL->Position = pos;
	pSL->Direction = dir;
	pSL->Attenuation = att;
	pSL->Range = r;
	pSL->SpotExp = spotExp;
}

void ShaderTextureLight::SendLightParameters(const Vect& eyepos)
{
	Data_LightParams dl;
	dl.PLcount = plCount;
	dl.SLcount = slCount;
	dl.DirLight = DirLightData;
	for (int i = 0; i < plCount; i++) {
		dl.PntLight[i] = PointLightData[i];
	}
	for (int i = 0; i < slCount; i++) {
		dl.SptLight[i] = SpotLightData[i];
	}
	dl.EyePosWorld = eyepos;
	dl.FogColor = FogColor;
	dl.FogRange = FogRange;
	dl.FogStart = FogStart;

	this->GetContext()->UpdateSubresource(mpBufferLightParams, 0, nullptr, &dl, 0, 0);
}

void ShaderTextureLight::SendWorldAndMaterial(const Matrix& world, const Vect& amb, const Vect& dif, const Vect& sp)
{
	WorldMatData mWorldData;
	mWorldData.World = world;
	mWorldData.WorldInv = world.getInv();
	mWorldData.Mat.Ambient = amb;
	mWorldData.Mat.Diffuse = dif;
	mWorldData.Mat.Specular = sp;

	this->GetContext()->UpdateSubresource(mpBufferWorldMatData, 0, nullptr, &mWorldData, 0, 0);
}

void ShaderTextureLight::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->VSSetConstantBuffers(2, 1, &mpBufferWorldMatData);

	devcon->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->PSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->PSSetConstantBuffers(2, 1, &mpBufferWorldMatData);
}