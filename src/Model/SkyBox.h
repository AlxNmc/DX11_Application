// Alex Niemiec 2020

#ifndef _SkyBox
#define _SkyBox

#include "Matrix.h"

class Model;
class Texture;
class ShaderTexture;
struct ID3D11DeviceContext;
struct ID3D11Device;

class SkyBox : public Align16
{
private:
	Model* pSkyBox;
	Texture* pTex;
	ShaderTexture* pShader;

public:
	SkyBox(const SkyBox&) = delete;				// Copy constructor
	SkyBox(Model&&) = delete;						// Move constructor
	SkyBox& operator=(const SkyBox&) & = delete;  // Copy assignment operator
	SkyBox& operator=(SkyBox&&) & = delete;       // Move assignment operator
	~SkyBox();

	SkyBox(ID3D11Device* dev, Texture* tex, ShaderTexture* shader, float size = 1000);

	void Render();
};

#endif _FlatPlane