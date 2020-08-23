// Alex Niemiec 2020

#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderColorLight.h"
#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	MeshColors = new Vect[n];
	for (int i = 0; i < n; i++)
		MeshColors[i] = Vect(0, 0, 0);
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{
	delete[] MeshColors;
}

void GraphicObject_ColorLight::SetColor(const Vect& col)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshColors[i] = col;
}

void GraphicObject_ColorLight::SetColor(const Vect& col, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshColors[meshnum] = col;
}

void GraphicObject_ColorLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_ColorLight::Render()
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, MeshColors[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}