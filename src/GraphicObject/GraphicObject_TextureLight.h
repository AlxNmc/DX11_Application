// Alex Niemiec 2020

#ifndef _GraphicObject_TextureLight
#define _GraphicObject_TextureLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderTextureLight.h"

class GraphicObject_TextureLight : public GraphicObject_Base
{
	friend class ShaderTextureLight;

	struct Material {
		Texture* pTex;
		Vect		amb;
		Vect		dif;
		Vect		sp;
	};

public:
	GraphicObject_TextureLight(const GraphicObject_TextureLight&) = delete;				 // Copy constructor
	GraphicObject_TextureLight(GraphicObject_TextureLight&&) = default;                    // Move constructor
	GraphicObject_TextureLight& operator=(const GraphicObject_TextureLight&) & = default;  // Copy assignment operator
	GraphicObject_TextureLight& operator=(GraphicObject_TextureLight&&) & = default;       // Move assignment operator
	~GraphicObject_TextureLight();		  											 // Destructor

	GraphicObject_TextureLight() = delete;

	void SetMaterial(Texture* tex, const Vect& amb, const Vect& dif, const Vect& sp);
	void SetMaterial(Texture* tex, const Vect& amb, const Vect& dif, const Vect& sp, int meshnum);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod);

private:
	ShaderTextureLight* pShader;
	Material* pMat;
	Matrix					World;
};

#endif _GraphicObject_TextureLight