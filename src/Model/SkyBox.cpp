// Alex Niemiec 2020

#include "SkyBox.h"
#include "Model.h"
#include "Texture.h"
#include "ShaderTexture.h"
#include "d3dUtil.h"

SkyBox::~SkyBox()
{
	delete pSkyBox;
}

SkyBox::SkyBox(ID3D11Device* dev, Texture* tex, ShaderTexture* shader, float size)
	:pTex(tex), pShader(shader)
{
	/************ set vertices and indeces *************/

	int nverts = 14;
	StandardVertex* pVerts = new StandardVertex[nverts];
	int ntri = 12;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	// top face
	pVerts[0].set(-size / 2, +size / 2, -size / 2, 0.5f, 0.375f);
	pVerts[1].set(+size / 2, +size / 2, -size / 2, 0.5f, 0.625f);
	pVerts[2].set(-size / 2, +size / 2, +size / 2, 0.25f, 0.375f);
	pVerts[3].set(+size / 2, +size / 2, +size / 2, 0.25f, 0.625f);

	pTriList[0].set(3, 2, 0);
	pTriList[1].set(1, 3, 0);

	// bottom face
	pVerts[4].set(-size / 2, -size / 2, -size / 2, 0.75f, 0.375f);
	pVerts[5].set(+size / 2, -size / 2, -size / 2, 0.75f, 0.625f);
	pVerts[6].set(-size / 2, -size / 2, +size / 2, 1, 0.375f);
	pVerts[7].set(+size / 2, -size / 2, +size / 2, 1, 0.625f);

	pTriList[2].set(5, 4, 6);
	pTriList[3].set(7, 5, 6);

	// front face
	pVerts[8].set(-size / 2, -size / 2, +size / 2, 0, 0.375f);
	pVerts[9].set(+size / 2, -size / 2, +size / 2, 0, 0.625f);

	pTriList[4].set(2, 3, 8);
	pTriList[5].set(3, 9, 8);

	// right face
	pVerts[10].set(-size / 2, -size / 2, -size / 2, 0.5f, 0.125f);
	pVerts[11].set(-size / 2, -size / 2, +size / 2, 0.25f, 0.125f);

	pTriList[6].set(0, 2, 11);
	pTriList[7].set(10, 0, 11);

	// left face
	pVerts[12].set(+size / 2, -size / 2, -size / 2, 0.5f, 0.875f);
	pVerts[13].set(+size / 2, -size / 2, +size / 2, 0.25f, 0.875f);

	pTriList[8].set(3, 1, 12);
	pTriList[9].set(13, 3, 12);

	// back face
	pTriList[10].set(1, 0, 4);
	pTriList[11].set(5, 1, 4);

	/**************** create model ******************/

	pSkyBox = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

void SkyBox::Render()
{
	pSkyBox->SetToContext(pShader->GetContext());
	pTex->SetToContext(pShader->GetContext());
	pShader->SendWorld(Matrix(IDENTITY));
	pSkyBox->Render(pShader->GetContext());
}
