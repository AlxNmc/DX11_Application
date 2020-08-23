// Alex Niemiec 2020

#include "ShaderTexture.h"
#include "d3dUtil.h"

ShaderTexture::ShaderTexture(ID3D11Device* device)
	: ShaderBase(device, L"../Assets/Shaders/Texture.hlsl")
{
	// Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElemetns = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElemetns);

	HRESULT hr;

	// camera data buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamData);
	assert(SUCCEEDED(hr));

	// world data buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WorldData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferWorldData);
	assert(SUCCEEDED(hr));

}

void ShaderTexture::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamData);
	devcon->VSSetConstantBuffers(1, 1, &mpBufferWorldData);
}

void ShaderTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamData mCamData;
	mCamData.View = view;
	mCamData.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamData, 0, nullptr, &mCamData, 0, 0);
}

void ShaderTexture::SendWorld(const Matrix& world)
{
	WorldData mWorldData;
	mWorldData.World = world;

	this->GetContext()->UpdateSubresource(mpBufferWorldData, 0, nullptr, &mWorldData, 0, 0);
}

ShaderTexture::~ShaderTexture()
{
	ReleaseAndDeleteCOMobject(mpBufferCamData);
	ReleaseAndDeleteCOMobject(mpBufferWorldData);
}
