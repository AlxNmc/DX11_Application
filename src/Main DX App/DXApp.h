// Alex Niemiec 2020

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "ShaderColorLight.h"
#include "ShaderTextureLight.h"

class Model;
class Texture;
class FlatPlane;
class TerrainModel;
class SkyBox;
class Mirror;
class ShaderTexture;
class GraphicObject_TextureLight;
class GraphicObject_ColorLight;

class DXApp : public Align16
{
private:
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations
	ID3D11RasterizerState* FrontFaceAsCCWRS;	// Default rasterizer state (used with mirror effect)

	GameTimer mTimer;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Debug utility
	void ReportLiveDXObjects();

	// ------------------Demo specific additions-----------------------------
		Camera mCam;

		// ---shaders---
		ShaderTextureLight* sh_textureLight;
		ShaderColorLight* sh_colorLight;
		Vect posPointFire;
		Vect posPointPortal;
		Vect posPointLamp;
		ShaderTexture* sh_skybox;

		// ---terrain---
		TerrainModel* pTerrain;
		Texture* tex_terrain;

		// ---skybox---
		SkyBox* pSkyBox;
		Texture* tex_skybox;

		// ---models---
			// house
			GraphicObject_TextureLight* pHouse;
			Model* mod_house;
			Texture* tex_house;
			Matrix m_house;

			// portal
			GraphicObject_TextureLight* pPortal;
			Model* mod_portal;
			Texture* tex_portal;
			Matrix m_portal;

			// table
			GraphicObject_TextureLight* pTable;
			Model* mod_table;
			Texture* tex_table;
			Matrix m_table;

			// candle
			GraphicObject_TextureLight* pCandle;
			Model* mod_candle;
			Texture* tex_candle1;
			Texture* tex_candle2;
			Texture* tex_candle3;
			Matrix m_candle;

			// cheese
			GraphicObject_TextureLight* pCheese;
			Model* mod_cheese;
			Texture* tex_cheese;
			Matrix m_cheese;

			// firepit
			GraphicObject_TextureLight* pFirepit;
			Model* mod_firepit;
			Texture* tex_firepit;
			Matrix m_firepit;

			// firelog
			GraphicObject_TextureLight* pFirelogs;
			Model* mod_firelogs;
			Texture* tex_firelogs;
			Matrix m_firelogs;

			// cauldron
			GraphicObject_TextureLight* pCauldron;
			Model* mod_cauldron;
			Texture* tex_cauldron;
			Matrix m_cauldron;

			// spider
			GraphicObject_TextureLight* pSpider;
			Model* mod_spider;
			Texture* tex_spider;
			Matrix m_spider;

			// dragon
			GraphicObject_TextureLight* pDragon;
			Model* mod_dragon;
			Texture* tex_dragon1;
			Texture* tex_dragon2;
			Matrix m_dragon;

			// trees
			GraphicObject_TextureLight* pTrees;
			Model* mod_trees;
			Texture* tex_trees;
			Matrix m_trees;

			Vect portalCenter;

			// cube
			GraphicObject_TextureLight* pCube;
			Model* mod_cube;
			Texture* tex_cube;
			Matrix m_cube;

			// single face cube
			GraphicObject_TextureLight* pCubeSF;
			Model* mod_cubeSF;
			Texture* tex_cubeSF;
			Matrix m_cubeSF;

			// pyramid
			GraphicObject_TextureLight* pPyramidSF;
			Model* mod_pyramidSF;
			Texture* tex_pyramidSF;
			Matrix m_pyramidSF;

			// sphere
			GraphicObject_ColorLight* pSphere;
			Model* mod_sphere;
			Matrix m_sphere;
			Vect col_sphere;

	// ------------------Demo specific additions-----------------------------

	// gimmicky mouse control
	Vect MousePos;
	float mTheta;
	float mPhi;
	float mRadius;

public:
	DXApp(HWND hwnd);
	virtual ~DXApp();

	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);
	void OnMouseWheel(short delta);


};

#endif _DXApp