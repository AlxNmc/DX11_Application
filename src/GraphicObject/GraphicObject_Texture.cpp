// Alex Niemiec 2020

#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderTexture.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);
	int n = mod->GetMeshCount();
	pTex = new Texture*[n];
}

GraphicObject_Texture::~GraphicObject_Texture()
{
	delete[] pTex;
}

void GraphicObject_Texture::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		pTex[i] = tex;
	}
}

void GraphicObject_Texture::SetTexture(Texture* tex, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	pTex[meshnum] = tex;
}

void GraphicObject_Texture::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Texture::Render()
{
	pModel->SetToContext(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pTex[i]->SetToContext(pShader->GetContext());
		pShader->SendWorld(World);
		pModel->Render(pShader->GetContext());
	}
}