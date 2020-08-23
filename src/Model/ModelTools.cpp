// Alex Niemiec 2020

#include "ModelTools.h"
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"

void ModelTools::CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 1, 0, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 1, 0, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 1, 1, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxSixFaceTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	// top face
	pVerts[0].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, 0, 1, 0);
	pVerts[1].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 0, 1, 0);
	pVerts[2].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, 0, 1, 0);
	pVerts[3].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 0, 1, 0);


	pTriList[0].set(0, 2, 3);
	pTriList[1].set(0, 3, 1);

	// bottom face
	pVerts[4].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.375f, 0, -1, 0);
	pVerts[5].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.625f, 0, -1, 0);
	pVerts[6].set(-0.5f, -0.5f, +0.5f, 1, 0.375f, 0, -1, 0);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, 1, 0.625f, 0, -1, 0);

	pTriList[2].set(6, 4, 5);
	pTriList[3].set(6, 5, 7);

	// front face
	pVerts[8].set(-0.5f, -0.5f, +0.5f, 0, 0.375f, 0, 0, 1);
	pVerts[9].set(+0.5f, -0.5f, +0.5f, 0, 0.625f, 0, 0, 1);
	pVerts[10].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, 0, 0, 1);
	pVerts[11].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 0, 0, 1);

	pTriList[4].set(8, 11, 10);
	pTriList[5].set(8, 9, 11);

	// right face
	pVerts[12].set(-0.5f, -0.5f, -0.5f, 0.5f, 0.125f, -1, 0, 0);
	pVerts[13].set(-0.5f, -0.5f, +0.5f, 0.25f, 0.125f, -1, 0, 0);
	pVerts[14].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, -1, 0, 0);
	pVerts[15].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, -1, 0, 0);

	pTriList[6].set(13, 15, 14);
	pTriList[7].set(13, 14, 12);

	// left face
	pVerts[16].set(+0.5f, -0.5f, -0.5f, 0.5f, 0.875f, 1, 0, 0);
	pVerts[17].set(+0.5f, -0.5f, +0.5f, 0.25f, 0.875f, 1, 0, 0);
	pVerts[18].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 1, 0, 0);
	pVerts[19].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 1, 0, 0);

	pTriList[8].set(16, 18, 19);
	pTriList[9].set(16, 19, 17);

	// back face
	pVerts[20].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, 0, 0, -1);
	pVerts[21].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 0, 0, -1);
	pVerts[22].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.375f, 0, 0, -1);
	pVerts[23].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.625f, 0, 0, -1);

	pTriList[10].set(22, 20, 21);
	pTriList[11].set(22, 21, 23);
}

void ModelTools::UnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 18;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	// front face
	pVerts[0].set(0.0f, 0.5f, 0.0f, 0.5f, 0);
	pVerts[1].set(-0.5f, -0.5f, 0.5f, 0, 1);
	pVerts[2].set(0.5f, -0.5f, 0.5f, 1, 1);
	Vect a = pVerts[1].Pos - pVerts[0].Pos;
	Vect b = pVerts[2].Pos - pVerts[0].Pos;
	Vect norm = (a.cross(b)).getNorm();
	pVerts[0].normal = norm;
	pVerts[1].normal = norm;
	pVerts[2].normal = norm;
	pTriList[0].set(0, 1, 2);

	// back face
	pVerts[3].set(0.0f, 0.5f, 0.0f, 0.5f, 0);
	pVerts[4].set(+0.5f, -0.5f, -0.5f, 0, 1);
	pVerts[5].set(-0.5f, -0.5f, -0.5f, 1, 1);
	a = pVerts[4].Pos - pVerts[3].Pos;
	b = pVerts[5].Pos - pVerts[3].Pos;
	norm = (a.cross(b)).getNorm();
	pVerts[3].normal = norm;
	pVerts[4].normal = norm;
	pVerts[5].normal = norm;
	pTriList[1].set(3, 4, 5);

	// right face
	pVerts[6].set(0.0f, 0.5f, 0.0f, 0.5f, 0);
	pVerts[7].set(0.5f, -0.5f, 0.5f, 0, 1);
	pVerts[8].set(+0.5f, -0.5f, -0.5f, 1, 1);
	a = pVerts[7].Pos - pVerts[6].Pos;
	b = pVerts[8].Pos - pVerts[6].Pos;
	norm = (a.cross(b)).getNorm();
	pVerts[6].normal = norm;
	pVerts[7].normal = norm;
	pVerts[8].normal = norm;
	pTriList[2].set(6, 7, 8);

	// left face
	pVerts[9].set(0.0f, 0.5f, 0.0f, 0.5f, 0);
	pVerts[10].set(-0.5f, -0.5f, -0.5f, 0, 1);
	pVerts[11].set(-0.5f, -0.5f, 0.5f, 1, 1);
	a = pVerts[10].Pos - pVerts[9].Pos;
	b = pVerts[11].Pos - pVerts[9].Pos;
	norm = (a.cross(b)).getNorm();
	pVerts[9].normal = norm;
	pVerts[10].normal = norm;
	pVerts[11].normal = norm;
	pTriList[3].set(9, 10, 11);

	// bottom front
	pVerts[12].set(0.5f, -0.5f, 0.5f, 1, 0); //fr
	pVerts[13].set(-0.5f, -0.5f, 0.5f, 0, 0); //fl
	pVerts[14].set(-0.5f, -0.5f, -0.5f, 0, 1); //bl
	a = pVerts[13].Pos - pVerts[12].Pos;
	b = pVerts[14].Pos - pVerts[12].Pos;
	norm = (a.cross(b)).getNorm();
	pVerts[12].normal = norm;
	pVerts[13].normal = norm;
	pVerts[14].normal = norm;
	pTriList[4].set(12, 13, 14);

	// bottom back
	pVerts[15].set(-0.5f, -0.5f, -0.5f, 0, 1); //bl
	pVerts[16].set(0.5f, -0.5f, -0.5f, 1, 1); //br
	pVerts[17].set(0.5f, -0.5f, 0.5f, 1, 0); //fr
	a = pVerts[16].Pos - pVerts[15].Pos;
	b = pVerts[17].Pos - pVerts[15].Pos;
	norm = (a.cross(b)).getNorm();
	pVerts[15].normal = norm;
	pVerts[16].normal = norm;
	pVerts[17].normal = norm;
	pTriList[5].set(15, 16, 17);

}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 2 + 2 * vslice * hslice;
	pVerts = new StandardVertex[nverts];
	ntri = 4 * vslice * hslice;
	pTriList = new TriangleByIndex[ntri];

#ifndef M_PI
#	define M_PI 3.14159265358979323846f
#endif

	int hsegs = 2 * vslice;
	int vsegs = hslice + 1;
	float v_angle = M_PI / vsegs;
	float h_angle = 2 * M_PI / hsegs;

	// ---------- get verticies ------------
	pVerts[0].set(0, -1, 0);
	pVerts[0].normal = pVerts[0].Pos;
	Vect last = pVerts[0].Pos;
	for (int i = 1; i < nverts - 1;)
	{
		pVerts[i].meshNum = 0;
		pVerts[i].Pos = last * Matrix(ROT_X, v_angle);
		pVerts[i].normal = pVerts[i].Pos;
		pVerts[i].Color = Colors::White;
		last = pVerts[i].Pos;
		i++;
		// get vertices for the horizontal slice
		for (int j = 0; j < hsegs - 1; j++, i++)
		{
			Vect pos = pVerts[i - 1].Pos * Matrix(ROT_Y, h_angle);
			pVerts[i].meshNum = 0;
			pVerts[i].set(pos, pos, Colors::White);
		}
	}
	pVerts[nverts - 1].meshNum = 0;
	pVerts[nverts - 1].Pos = last * Matrix(ROT_X, v_angle);
	pVerts[nverts - 1].normal = pVerts[nverts - 1].Pos;
	pVerts[nverts - 1].Color = Colors::White;

	// ----------- get indicies -------------
	int i = 0;
	// wind bottom cap
	for (int j = 2; i < ntri && j <= hsegs; j++, i++)
	{
		pTriList[i].set(0, j, j - 1);
	}
	if (i < ntri) pTriList[i].set(0, 1, hsegs);
	i++;

	// wind middle band
	for (int j = 0; i < ntri && j < vsegs - 2; j++, i += 2)
	{
		int low = 1 + j * hsegs;
		int high = 1 + (j + 1) * hsegs;
		for (int k = 0; k < hsegs - 1; k++, low++, high++, i += 2)
		{
			pTriList[i].set(low, high + 1, high);
			pTriList[i + 1].set(low, low + 1, high + 1);
		}
		pTriList[i].set(low, 1 + (j + 1) * hsegs, high);
		pTriList[i + 1].set(low, 1 + j * hsegs, 1 + (j + 1) * hsegs);
	}

	// wind top cap
	for (int j = nverts - 3; i < ntri && j > nverts - (hsegs + 2); j--, i++)
	{
		pTriList[i].set(nverts - 1, j, j + 1);
	}
	if (i < ntri) pTriList[i].set(nverts - 1, nverts - 2, nverts - (hsegs + 1));
}
