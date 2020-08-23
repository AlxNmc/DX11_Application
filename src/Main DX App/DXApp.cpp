// Alex Niemiec 2020

#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include "d3dUtil.h"

// needed to load shaders from file
#include <d3dcompiler.h>

#include "Model.h"
#include "ShaderTexture.h"
#include "DirectXTex.h"
#include "Texture.h"
#include "FlatPlane.h"
#include "TerrainModel.h"
#include "Skybox.h"
#include "Mirror.h"
#include "GraphicObject_TextureLight.h"
#include "GraphicObject_ColorLight.h"

void DXApp::InitDemo()
{
	// texture shader
	Vect skyColor = Vect(.239f, .267f, .337f, 1);
	posPointFire = Vect(0, 3, -10);
	posPointLamp = Vect(0, 10, -30);
	posPointPortal = Vect(0, 19, 45);
	sh_textureLight = new ShaderTextureLight(md3dDevice, 0, 3, 15, 150, skyColor);
	sh_textureLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(), .1 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));
	sh_textureLight->SetPointLightParameters(0, posPointFire, 100, .2 * Vect(0, 1, 0), .1 * Vect(1, 1, 1), 2 * Colors::OrangeRed, 1 * Colors::OrangeRed);
	sh_textureLight->SetPointLightParameters(1, posPointLamp, 100, .2 * Vect(0, 1, 0), .1 * Vect(1, 1, 1), 2 * Colors::Orange, 1 * Colors::Orange);
	sh_textureLight->SetPointLightParameters(2, posPointPortal, 100, .2 * Vect(0, 1, 0), .1 * Vect(1, 1, 1), 4 * Colors::BlueViolet, 1 * Colors::BlueViolet);

	// color shader
	sh_colorLight = new ShaderColorLight(md3dDevice, 0, 3, 15, 150, skyColor);
	sh_colorLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(), .1 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));
	sh_colorLight->SetPointLightParameters(0, posPointFire, 100, .2 * Vect(0, 1, 0), .1 * Vect(1, 1, 1), 2 * Colors::OrangeRed, 1 * Colors::OrangeRed);
	sh_colorLight->SetPointLightParameters(1, posPointLamp, 100, .2 * Vect(0, 1, 0), .1 * Vect(1, 1, 1), 2 * Colors::Orange, 1 * Colors::Orange);
	sh_colorLight->SetPointLightParameters(2, posPointPortal, 100, .2 * Vect(0, 1, 0), .1 * Vect(1, 1, 1), 4 * Colors::BlueViolet, 1 * Colors::BlueViolet);

	// terrain
	tex_terrain = new Texture(md3dDevice, L"../Assets/Textures/cracked_dirt_texture.tga", D3D11_FILTER_ANISOTROPIC, 16, true, 0, DirectX::TEX_FILTER_POINT);
	pTerrain = new TerrainModel(md3dDevice, sh_textureLight, L"../Assets/Textures/final_heightmap.tga", 700, 60, -27, 30, 30);
	pTerrain->SetTexture(tex_terrain);

	// skybox
	sh_skybox = new ShaderTexture(md3dDevice);
	tex_skybox = new Texture(md3dDevice, L"../Assets/Textures/SkyBoxTexture.tga");
	pSkyBox = new SkyBox(md3dDevice, tex_skybox, sh_skybox);

	// house
	m_house =Matrix(ROT_Y, 1.6f) * Matrix(TRANS, Vect(50, 0, -30));
	mod_house = new Model(md3dDevice, "../Assets/Models/house.azul");
	tex_house = new Texture(md3dDevice, L"../Assets/Textures/house.tga");
	pHouse = new GraphicObject_TextureLight(sh_textureLight, mod_house);
	pHouse->SetMaterial(tex_house, Colors::White, Colors::White, Vect(0, 0, 0, 100));
	pHouse->SetWorld(m_house);

	// portal
	m_portal = Matrix(TRANS, Vect(0, 12, 50));
	mod_portal = new Model(md3dDevice, "../Assets/Models/portal.azul");
	tex_portal = new Texture(md3dDevice, L"../Assets/Textures/portal.tga");
	pPortal = new GraphicObject_TextureLight(sh_textureLight, mod_portal);
	pPortal->SetMaterial(tex_portal, Colors::White, Colors::White, Vect(0, 0, 0, 100));
	pPortal->SetWorld(m_portal);

	// table
	m_table = Matrix(TRANS, Vect(0, 1, -30));
	mod_table = new Model(md3dDevice, "../Assets/Models/table.azul");
	tex_table = new Texture(md3dDevice, L"../Assets/Textures/table.tga");
	pTable = new GraphicObject_TextureLight(sh_textureLight, mod_table);
	pTable->SetMaterial(tex_table, Colors::White, Colors::White, Vect(0, 0, 0, 100));
	pTable->SetWorld(m_table);

	// candle
	m_candle = m_table;
	mod_candle = new Model(md3dDevice, "../Assets/Models/candle.azul");
	tex_candle1 = new Texture(md3dDevice, L"../Assets/Textures/candle1.tga");
	tex_candle2 = new Texture(md3dDevice, L"../Assets/Textures/candle2.tga");
	tex_candle3 = new Texture(md3dDevice, L"../Assets/Textures/candle3.tga");
	pCandle = new GraphicObject_TextureLight(sh_textureLight, mod_candle);
	pCandle->SetMaterial(tex_candle1, Colors::White, Colors::White, Vect(1, 1, 1, 100), 0);
	pCandle->SetMaterial(tex_candle2, Colors::White, Colors::White, Vect(1, 1, 1, 100), 1);
	pCandle->SetMaterial(tex_candle3, Colors::White, Colors::White, Vect(1, 1, 1, .1f), 2);
	pCandle->SetWorld(m_candle);

	// cheese
	m_cheese = m_table;
	mod_cheese = new Model(md3dDevice, "../Assets/Models/cheese.azul");
	tex_cheese = new Texture(md3dDevice, L"../Assets/Textures/cheese.tga");
	pCheese = new GraphicObject_TextureLight(sh_textureLight, mod_cheese);
	pCheese->SetMaterial(tex_cheese, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pCheese->SetWorld(m_cheese);

	// firepit
	m_firepit = Matrix(TRANS, Vect(0, 1, -10));
	mod_firepit = new Model(md3dDevice, "../Assets/Models/firepit.azul");
	tex_firepit = new Texture(md3dDevice, L"../Assets/Textures/firepit.tga");
	pFirepit = new GraphicObject_TextureLight(sh_textureLight, mod_firepit);
	pFirepit->SetMaterial(tex_firepit, Colors::White, Colors::White, 0 * Vect(1, 1, 1, 100));
	pFirepit->SetWorld(m_firepit);

	// firelogs
	m_firelogs = m_firepit * Matrix(TRANS, Vect(1, 1, 0));
	mod_firelogs = new Model(md3dDevice, "../Assets/Models/firelogs.azul");
	tex_firelogs = new Texture(md3dDevice, L"../Assets/Textures/firelogs.tga");
	pFirelogs = new GraphicObject_TextureLight(sh_textureLight, mod_firelogs);
	pFirelogs->SetMaterial(tex_firelogs, Colors::White, Colors::White, 0 * Vect(1, 1, 1, 100));
	pFirelogs->SetWorld(m_firelogs);

	// cauldron
	m_cauldron = m_firepit * Matrix(TRANS, Vect(0, 2, 0));
	mod_cauldron = new Model(md3dDevice, "../Assets/Models/cauldron.azul");
	tex_cauldron = new Texture(md3dDevice, L"../Assets/Textures/cauldron.tga");
	pCauldron = new GraphicObject_TextureLight(sh_textureLight, mod_cauldron);
	pCauldron->SetMaterial(tex_cauldron, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pCauldron->SetWorld(m_cauldron);

	// spider
	m_spider = Matrix(ROT_X, -.2) * Matrix(TRANS, Vect(0, 1, 0));
	mod_spider = new Model(md3dDevice, "../Assets/Models/spider.azul");
	tex_spider = new Texture(md3dDevice, L"../Assets/Textures/spider.tga");
	pSpider = new GraphicObject_TextureLight(sh_textureLight, mod_spider);
	pSpider->SetMaterial(tex_spider, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pSpider->SetWorld(m_spider);

	// dragon
	m_dragon = Matrix(TRANS, Vect(100, 75, 0));
	mod_dragon = new Model(md3dDevice, "../Assets/Models/Dragon.azul");
	tex_dragon1 = new Texture(md3dDevice, L"../Assets/Textures/DragonMain.tga");
	tex_dragon2 = new Texture(md3dDevice, L"../Assets/Textures/DragonEyes.tga");
	pDragon = new GraphicObject_TextureLight(sh_textureLight, mod_dragon);
	pDragon->SetMaterial(tex_dragon1, .1f * Colors::White, Colors::White, 0.5f * Vect(1, 1, 1, 100), 0);
	pDragon->SetMaterial(tex_dragon2, Colors::White, Colors::White, Vect(1, 1, 1, 100), 1);
	pDragon->SetWorld(m_dragon);

	// trees
	m_trees = Matrix(TRANS, Vect(-40, 0, 70));
	mod_trees = new Model(md3dDevice, "../Assets/Models/trees.azul");
	tex_trees = new Texture(md3dDevice, L"../Assets/Textures/trees.tga");
	pTrees = new GraphicObject_TextureLight(sh_textureLight, mod_trees);
	pTrees->SetMaterial(tex_trees, Colors::White, Colors::White, 0 * Vect(1, 1, 1, 100));
	pTrees->SetWorld(m_trees);

	// sphere
	portalCenter = Vect(0, 19, 50);
	m_sphere = Matrix(TRANS, portalCenter);
	col_sphere = Colors::Black;
	mod_sphere = new Model(md3dDevice, Model::UnitSphere);
	pSphere = new GraphicObject_ColorLight(sh_colorLight, mod_sphere);
	pSphere->SetColor(col_sphere);
	pSphere->SetWorld(m_sphere);

	// cube
	m_cube = m_sphere * Matrix(TRANS, Vect(0, 0, -10));
	mod_cube = new Model(md3dDevice, Model::UnitBoxSixFaceTexture);
	tex_cube = new Texture(md3dDevice, L"../Assets/Textures/RuneWrappedCube.tga");
	pCube = new GraphicObject_TextureLight(sh_textureLight, mod_cube);
	pCube->SetMaterial(tex_cube, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pCube->SetWorld(m_cube);

	// single face cube
	m_cubeSF = m_sphere * Matrix(TRANS, Vect(8.66, 0, 5));
	mod_cubeSF = new Model(md3dDevice, Model::UnitBoxRepeatedTexture);
	tex_cubeSF = new Texture(md3dDevice, L"../Assets/Textures/PyramidWarppedTexture.tga");
	pCubeSF = new GraphicObject_TextureLight(sh_textureLight, mod_cubeSF);
	pCubeSF->SetMaterial(tex_cubeSF, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pCubeSF->SetWorld(m_cubeSF);

	// pyramid
	m_pyramidSF = m_sphere * Matrix(TRANS, Vect(-8.66, 0, 5));
	mod_pyramidSF = new Model(md3dDevice, Model::UnitPyramidRepTexture);
	tex_pyramidSF = new Texture(md3dDevice, L"../Assets/Textures/PyramidRuneTexture.tga");
	pPyramidSF = new GraphicObject_TextureLight(sh_textureLight, mod_pyramidSF);
	pPyramidSF->SetMaterial(tex_pyramidSF, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pPyramidSF->SetWorld(m_pyramidSF);

	// Initialize the projection matrix
	mCam.setPerspective(45, mClientWidth / (float)mClientHeight, 1.0f, 1000.0f);
	mCam.setOrientAndPosition(Vect(0, 1, 0), Vect(10, 15, 0), Vect(-10, 15, -60));

	// Initialize gimmicky mouse control values
	mTheta = .5;
	mPhi = 3.1415f / 8;
	mRadius = 6;

	mTimer.Reset();
}

void DXApp::UpdateScene()
{
	//------ world transformations -----
	m_dragon *= Matrix(ROT_Y, .0001f);

	m_cube *= Matrix(TRANS, -portalCenter);
	m_cube *= Matrix(ROT_Y, .0001f);
	m_cube *= Matrix(TRANS, portalCenter);

	m_pyramidSF *= Matrix(TRANS, -portalCenter);
	m_pyramidSF *= Matrix(ROT_Y, .0001f);
	m_pyramidSF *= Matrix(TRANS, portalCenter);

	m_cubeSF *= Matrix(TRANS, -portalCenter);
	m_cubeSF *= Matrix(ROT_Y, .0001f);
	m_cubeSF *= Matrix(TRANS, portalCenter);
	//----------------------------------

	float camSpeed = 0.004f;
	if (GetKeyState('W') & 0x08000)
	{
		mCam.TranslateFwdBack(camSpeed);
	}
	else if (GetKeyState('S') & 0x08000)
	{
		mCam.TranslateFwdBack(-camSpeed);
	}

	if (GetKeyState('A') & 0x08000)
	{
		mCam.TranslateLeftRight(-camSpeed);
	}
	else if (GetKeyState('D') & 0x08000)
	{
		mCam.TranslateLeftRight(camSpeed);
	}

	float rotSpeed = 0.0005f;
	if (GetKeyState(VK_LEFT) & 0x08000)
	{
		mCam.TurnLeftRight(rotSpeed);
	}
	else if (GetKeyState(VK_RIGHT) & 0x08000)
	{
		mCam.TurnLeftRight(-rotSpeed);
	}

	if (GetKeyState(VK_UP) & 0x08000)
	{
		mCam.TiltUpDown(rotSpeed);
	}
	else if (GetKeyState(VK_DOWN) & 0x08000)
	{
		mCam.TiltUpDown(-rotSpeed);
	}

	mCam.updateCamera();
}

void DXApp::DrawScene()
{
	// Clear the back buffer and depth stencil
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// current camera position
	Vect eyepos;
	mCam.getPos(eyepos);

	// update world matrices
	pDragon->SetWorld(m_dragon);
	pCauldron->SetWorld(m_cauldron * Matrix(TRANS, 0, .5 * cosf(mTimer.TotalTime()), 0));
	pCube->SetWorld(m_cube);
	pCubeSF->SetWorld(m_cubeSF);
	pPyramidSF->SetWorld(m_pyramidSF);

	// lit texture shader
	sh_textureLight->SetToContext(md3dImmediateContext);
	sh_textureLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	sh_textureLight->SendLightParameters(eyepos);

		pTerrain->Render();
		pHouse->Render();
		pPortal->Render();
		pTable->Render();
		pCandle->Render();
		pCheese->Render();
		pFirepit->Render();
		pFirelogs->Render();
		pCauldron->Render();
		pSpider->Render();
		pDragon->Render();
		pTrees->Render();
		pCube->Render();
		pCubeSF->Render();
		pPyramidSF->Render();

	// unlit texture shader
	sh_skybox->SetToContext(md3dImmediateContext);
	sh_skybox->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

		pSkyBox->Render();

	// lit color shader
	sh_colorLight->SetToContext(md3dImmediateContext);
	sh_colorLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	sh_colorLight->SendLightParameters(eyepos);

	pSphere->Render();

	// Switches the display to show the now-finished back-buffer
	mSwapChain->Present(0, 0);
}


DXApp::DXApp(HWND hwnd)
{
	assert(hwnd);
	mhMainWnd = hwnd;

	BackgroundColor = Colors::MidnightBlue;

	md3dDevice = nullptr;
	md3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	CHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);
	mMainWndCaption = str;

	// Initialize DX11
	this->InitDirect3D();

	// Demo initialization
	this->InitDemo();
}

DXApp::~DXApp()
{
	delete sh_textureLight;
	delete sh_colorLight;
	delete sh_skybox;

	delete pTerrain;
	delete pSkyBox;
	delete pHouse;
	delete pPortal;
	delete pTable;
	delete pCandle;
	delete pCheese;
	delete pFirepit;
	delete pFirelogs;
	delete pCauldron;
	delete pSpider;
	delete pDragon;
	delete pTrees;
	delete pCube;
	delete pCubeSF;
	delete pPyramidSF;
	delete pSphere;

	delete mod_house;
	delete mod_portal;
	delete mod_table;
	delete mod_candle;
	delete mod_cheese;
	delete mod_firepit;
	delete mod_firelogs;
	delete mod_cauldron;
	delete mod_spider;
	delete mod_dragon;
	delete mod_trees;
	delete mod_cube;
	delete mod_cubeSF;
	delete mod_pyramidSF;
	delete mod_sphere;

	delete tex_house;
	delete tex_portal;
	delete tex_table;
	delete tex_candle1;
	delete tex_candle2;
	delete tex_candle3;
	delete tex_cheese;
	delete tex_firepit;
	delete tex_firelogs;
	delete tex_cauldron;
	delete tex_spider;
	delete tex_dragon1;
	delete tex_dragon2;
	delete tex_trees;
	delete tex_cube;
	delete tex_cubeSF;
	delete tex_pyramidSF;
	delete tex_terrain;
	delete tex_skybox;

	ReleaseAndDeleteCOMobject(FrontFaceAsCCWRS);
	ReleaseAndDeleteCOMobject(mRenderTargetView);
	ReleaseAndDeleteCOMobject(mpDepthStencilView);
	ReleaseAndDeleteCOMobject(mSwapChain);
	ReleaseAndDeleteCOMobject(md3dImmediateContext);

	// Must be done BEFORE the device is released
	ReportLiveDXObjects();		// See http://masterkenth.com/directx-leak-debugging/

	ReleaseAndDeleteCOMobject(md3dDevice);
}

// See http://masterkenth.com/directx-leak-debugging/
void DXApp::ReportLiveDXObjects()
{
#ifdef _DEBUG
	HRESULT hr = S_OK;

	// Now we set up the Debug interface, to be queried on shutdown
	ID3D11Debug* debugDev;
	hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));
	assert(SUCCEEDED(hr));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ReleaseAndDeleteCOMobject(debugDev);
#endif
}

void DXApp::InitDirect3D()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This is a *greatly* simplified process to create a DX device and context:
	// We force the use of DX11 feature level since that's what CDM labs are limited to.
	// For real-life applications would need to test what's the best feature level and act accordingly
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &md3dDevice, nullptr, &md3dImmediateContext);
	assert(SUCCEEDED(hr));

	// Now we obtain the associated DXGIfactory1 with our device 
	// Many steps...
	IDXGIDevice* dxgiDevice = nullptr;
	hr = md3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = nullptr;
	hr = dxgiDevice->GetAdapter(&adapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory1* dxgiFactory1 = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory1));
	assert(SUCCEEDED(hr));
	// See also note on weird stuff with factories and swap chains (1s and 2s)
	// https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

	// We are done with these now...
	ReleaseAndDeleteCOMobject(adapter);
	ReleaseAndDeleteCOMobject(dxgiDevice);

	// Controls MSAA option:
	// - 4x count level garanteed for all DX11 
	// - MUST be the same for depth buffer!
	// - We _need_ to work with the depth buffer because reasons... (see below)
	DXGI_SAMPLE_DESC sampDesc;
	sampDesc.Count = 1;
	sampDesc.Quality = static_cast<UINT>(D3D11_CENTER_MULTISAMPLE_PATTERN);  // MS: what's with the type mismtach?

	DXGI_MODE_DESC buffdesc;				// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173064(v=vs.85).aspx
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Next we create a swap chain. 
	// Useful thread: http://stackoverflow.com/questions/27270504/directx-creating-the-swapchain
	// Note that this is for a DirectX 11.0: in a real app, we should test the feature levels and act accordingly

	DXGI_SWAP_CHAIN_DESC sd;				// See MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173075(v=vs.85).aspx
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;						// Much confusion about this number... see http://www.gamedev.net/topic/633807-swap-chain-buffer-count/
	sd.BufferDesc = buffdesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mhMainWnd;
	sd.SampleDesc = sampDesc;
	sd.Windowed = TRUE;

	hr = dxgiFactory1->CreateSwapChain(md3dDevice, &sd, &mSwapChain);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(dxgiFactory1);

	// Create a render target view		https://msdn.microsoft.com/en-us/library/windows/desktop/ff476582(v=vs.85).aspx
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	assert(SUCCEEDED(hr));;

	hr = md3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	ReleaseAndDeleteCOMobject(pBackBuffer);
	assert(SUCCEEDED(hr));

	/**********************************************************/

	// First we fix what it means for triangles to be front facing.
	// Requires setting a whole new rasterizer state
	//*
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;  // Also: D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = true; // true for RH forward facing
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;  // Does not in fact turn on/off multisample: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476198(v=vs.85).aspx
	rd.AntialiasedLineEnable = false;

	//ID3D11RasterizerState* rs;
	md3dDevice->CreateRasterizerState(&rd, &FrontFaceAsCCWRS);

	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);
	//ReleaseAndDeleteCOMobject(rs); // we can release this resource since we won't be changing it any further
	//*/

	// We must turn on the abilty to process depth during rendering.
	// Done through depth stencils (see https://msdn.microsoft.com/en-us/library/windows/desktop/bb205074(v=vs.85).aspx)
	// Below is a simplified version
	//*
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = mClientWidth;
	descDepth.Height = mClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = sampDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencil;
	hr = md3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	assert(SUCCEEDED(hr));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;;

	hr = md3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mpDepthStencilView);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(pDepthStencil);
	//*/

	/**********************************************************/

	//md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);  // to use without depth stencil
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mpDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)mClientWidth;
	vp.Height = (FLOAT)mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	md3dImmediateContext->RSSetViewports(1, &vp);
}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::FrameTick()
{
	mTimer.Tick();
	CalculateFrameStats();

	this->UpdateScene();
	this->DrawScene();
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	// Gimmicky mouse control
	if (btnState & MK_LBUTTON)
	{
		MousePos[x] = static_cast<float>(xval);
		MousePos[y] = static_cast<float>(yval);
	}
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void  DXApp::OnMouseWheel(short delta)
{
	if (delta > 0)
	{
		mRadius -= 1;
	}
	else
	{
		mRadius += 1;
	}
}


// Shader loading utility. Will be moved elsewhere later...
// Needs to be moved. Requires
HRESULT DXApp::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}