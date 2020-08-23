// Alex Niemiec 2020

#include "Mirror.h"
#include "FlatPlane.h"
#include "ShaderColorLight.h"

Mirror::~Mirror()
{
	delete pShader;

	ReleaseAndDeleteCOMobject(MarkMirrorDSS);
	ReleaseAndDeleteCOMobject(DrawReflectionDSS);
	ReleaseAndDeleteCOMobject(MirrorFrontFaceAsClockWiseRS);
	ReleaseAndDeleteCOMobject(TransparentBS);
	ReleaseAndDeleteCOMobject(NoWriteToRenderTargetBS);
}

Mirror::Mirror(ID3D11Device& md3dDevice, ShaderColorLight* shader, ID3D11RasterizerState& defaultRasterizerState, FlatPlane* pMirror, Matrix mirrorMat, Vect mirrorColor)
	:pShader(shader), pMirror(pMirror), mirrorMat(mirrorMat), mirrorColor(mirrorColor)
{
	FrontFaceAsCCWRS = &defaultRasterizerState;

	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	ZeroMemory(&mirrorDesc, sizeof(mirrorDesc));
	mirrorDesc.DepthEnable = true;								// Use the depth test
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	// but don't write to the depth buffer
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;							// Use the stencil test
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// When passing the test (Always) replace
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;	// the color value with the reference value (int 1)


	// Back faces are culled, so these don't matter 
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = md3dDevice.CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	ZeroMemory(&drawReflectionDesc, sizeof(drawReflectionDesc));
	drawReflectionDesc.DepthEnable = true;								// Use the depth test
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// normally
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;							// Use the stencil test
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// When passing the stencil comparison test,
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// do not modify the stencil

	// Back faces are culled, so these don't matter
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = md3dDevice.CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS);
	assert(SUCCEEDED(hr));

	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;   // D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;
	rd.AntialiasedLineEnable = false;

	hr = md3dDevice.CreateRasterizerState(&rd, &MirrorFrontFaceAsClockWiseRS);
	assert(SUCCEEDED(hr));

	// These settings implement the standard alpha blending: c_src*(a_src) + c_dst*(1-a_src) 
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = md3dDevice.CreateBlendState(&bd, &TransparentBS);
	assert(SUCCEEDED(hr));

	// These setting prevent all writings to the render target 
	// (the key is that RenderTargetWriteMask = 0, so nothing is allowed)
	// see https://msdn.microsoft.com/en-us/library/windows/desktop/bb204901(v=vs.85).aspx
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = false;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0;

	hr = md3dDevice.CreateBlendState(&bd, &NoWriteToRenderTargetBS);
	assert(SUCCEEDED(hr));

	// Create Reflection matrix for mirror plane.
	// Uses the mirror's normal N and position
	Vect N = mirrorMat.get(ROW_1); // Mirror normal
	Vect p = mirrorMat.get(ROW_3); // point on mirror
	float d = -p.dot(N);
	Matrix NTN(N[x] * N, N[y] * N, N[z] * N, d * N);  // Set the 4 rows
	NTN[m3] = 0;	// Zero out the #$#@$! 1 that's defaulted in the w position
	NTN[m7] = 0;
	NTN[m11] = 0;
	ReflectionMat = Matrix(IDENTITY) - 2 * NTN;
	ReflectionMat[m15] = 1; // correcting the corner entry
	// See also: https://en.wikipedia.org/wiki/Transformation_matrix
	// and http://ami.ektf.hu/uploads/papers/finalpdf/AMI_40_from175to186.pdf
	// And of course http://facweb.cs.depaul.edu/andre/GAM325/Week3.htm
}

void Mirror::enableMirrorRendering(ID3D11DeviceContext* context)
{
	// BLEND STATE: Stop writing to the render target (but keep depth writing to depth buffer)
	context->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);

	// STENCIL: Set up the stencil for marking ('1' for all pixels that passed the test. See comment at line 35)
	context->OMSetDepthStencilState(MarkMirrorDSS, 1);

	// Render the mirror 
	pShader->SetToContext(context);
	pShader->SendWorldAndMaterial(mirrorMat); // The color is irrelevant here
	pMirror->Render(context);

	// STENCIL: stop using the stencil
	context->OMSetDepthStencilState(0, 0);

	// BLEND STATE: Return the blend state to normal (writing to render target)
	context->OMSetBlendState(0, nullptr, 0xffffffff);

	// WINDINGS: face winding will appear inside out after reflection. Switching to CW front facing
	context->RSSetState(MirrorFrontFaceAsClockWiseRS);
	// STENCIL: Use the stencil test (reference value 1) and only pass the test if the stencil already had a one present
	context->OMSetDepthStencilState(DrawReflectionDSS, 1);
}

void Mirror::finishMirror(ID3D11DeviceContext* context)
{
	context->OMSetDepthStencilState(0, 0);
	// back to normal windings
	context->RSSetState(FrontFaceAsCCWRS);

	// Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	context->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);

	// Render mirror
	pShader->SetToContext(context);
	pShader->SendWorldAndMaterial(mirrorMat, mirrorColor);
	pMirror->Render(context);

	// Back to normal blending (turn off transparent blending)
	context->OMSetBlendState(0, nullptr, 0xffffffff);
}

Matrix Mirror::getReflectionMatrix()
{
	return ReflectionMat;
}
