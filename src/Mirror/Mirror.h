// Alex Niemiec 2020

#ifndef _Mirror
#define _Mirror

#include <d3d11.h>
#include "d3dUtil.h"
#include "Camera.h"
#include "Matrix.h"

class FlatPlane;
class ShaderColorLight;

class Mirror {

private:

	FlatPlane* pMirror;
	Matrix mirrorMat;
	Matrix ReflectionMat;
	Vect mirrorColor;		// CAUTION!  You _must_ have alpha < 1 to have transparency
	
	ShaderColorLight* pShader;

	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;

	ID3D11RasterizerState* FrontFaceAsCCWRS;
	ID3D11RasterizerState* MirrorFrontFaceAsClockWiseRS;

	ID3D11BlendState* NoWriteToRenderTargetBS;
	ID3D11BlendState* TransparentBS;


public:
	Mirror(const Mirror&) = delete;				 // Copy constructor
	Mirror(Mirror&&) = default;                    // Move constructor
	Mirror& operator=(const Mirror&) & = default;  // Copy assignment operator
	Mirror& operator=(Mirror&&) & = default;       // Move assignment operator
	~Mirror();

	Mirror(ID3D11Device& md3dDevice, ShaderColorLight* shader, ID3D11RasterizerState& defaultRasterizerState, FlatPlane* pMirror, Matrix mirrorMat, Vect mirrorColor);

	void enableMirrorRendering(ID3D11DeviceContext* context);
	void finishMirror(ID3D11DeviceContext* context);
	Matrix getReflectionMatrix();
};

#endif _Mirror