// Alex Niemiec 2020

#include "GraphicObject_TextureLight.h"
#include "Model.h"
#include "ShaderTextureLight.h"
#include <assert.h>

GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	pMat = new Material[n];
}

GraphicObject_TextureLight::~GraphicObject_TextureLight()
{
	delete[] pMat;
}

void GraphicObject_TextureLight::SetMaterial(Texture* tex, const Vect& amb, const Vect& dif, const Vect& sp)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		pMat[i].pTex = tex;
		pMat[i].amb = amb;
		pMat[i].dif = dif;
		pMat[i].sp = sp;
	}
}

void GraphicObject_TextureLight::SetMaterial(Texture* tex, const Vect& amb, const Vect& dif, const Vect& sp, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	pMat[meshnum].pTex = tex;
	pMat[meshnum].amb = amb;
	pMat[meshnum].dif = dif;
	pMat[meshnum].sp = sp;
}

void GraphicObject_TextureLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_TextureLight::Render()
{
	pModel->SetToContext(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pMat[i].pTex->SetToContext(pShader->GetContext());
		pShader->SendWorldAndMaterial(World, pMat[i].amb, pMat[i].dif, pMat[i].sp);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}