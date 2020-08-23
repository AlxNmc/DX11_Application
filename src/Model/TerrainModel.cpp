// Alex Niemiec 2020

#include "TerrainModel.h"
#include "ShaderTextureLight.h"
#include "Texture.h"
#include "Model.h"
#include "DirectXTex.h"
#include "d3dUtil.h"

TerrainModel::TerrainModel(ID3D11Device* dev, ShaderTextureLight* shader, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, float RepeatU, float RepeatV)
	:pShader(shader)
{
	// parse TGA heightmap file
	DirectX::ScratchImage scrTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width);

	const int side = hgtmap->height;
	size_t pixel_width = 4;			// 4 bytes RGBA per pixel

	int nverts = side * side;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = (2 * nverts) - (4 * side) + 2;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	// generate verices
	// 0---1---2
	// |   |   |
	// 3---4---5
	// |   |   |
	// 6---7---8
	float vert_d = len / (side - 1);
	float u_d = RepeatU / (side - 1);
	float v_d = RepeatV / (side - 1);
	for (int i = 0; i < side; i++) 
	{
		float v = v_d * i;

		float z = (len / 2) - (i * vert_d);
		for (int j = 0; j < side; j++) 
		{
			float u = u_d * j;
			int index = i * side + j;
			float x = (len / 2) - (j * vert_d);
			float y = ((float)(hgtmap->pixels[TexelIndex(pixel_width, side, j, i)]) / 255) * maxheight;
			y += ytrans;
			pVerts[index].set(x, y, z, u, v, Colors::White);
		}
	}

	// generate normals for field vertices
	for (int i = 1; i < side - 1; i++) 
	{
		for (int j = 1; j < side - 1; j++) 
		{
			int index = i * side + j;
			Vect v1 = getFaceNormal(pVerts[index].Pos, pVerts[index - side].Pos, pVerts[index - 1].Pos);
			Vect v2 = getFaceNormal(pVerts[index].Pos, pVerts[index - 1].Pos, pVerts[index - 1 + side].Pos);
			Vect v3 = getFaceNormal(pVerts[index].Pos, pVerts[index - 1 + side].Pos, pVerts[index + side].Pos);
			Vect v4 = getFaceNormal(pVerts[index].Pos, pVerts[index + side].Pos, pVerts[index + 1].Pos);
			Vect v5 = getFaceNormal(pVerts[index].Pos, pVerts[index + 1].Pos, pVerts[index + 1 - side].Pos);
			Vect v6 = getFaceNormal(pVerts[index].Pos, pVerts[index + 1 - side].Pos, pVerts[index - side].Pos);
			pVerts[index].normal = (v1 + v2 + v3 + v4 + v5 + v6).getNorm();
		}
	}

	// generate indices
	// 0---1---2
	// | / | / |
	// 3---4---5
	// | / | / |
	// 6---7---8
	for (int i = 0, triIndex = 0; i < side - 1; i++) 
	{
		for (int j = 0; j < side - 1; j++, triIndex+=2) 
		{
			// left triangle in tile
			int val1 = (i * side) + j;
			int val2 = ((i + 1) * side) + j;
			int val3 = val1 + 1;
			pTriList[triIndex].set(val1, val2, val3);

			// right triangle in tile
			val1 = val2 + 1;
			pTriList[triIndex + 1].set(val2, val1, val3);
		}
	}

	pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);
	delete[] pVerts;
	delete[] pTriList;
}

int TerrainModel::TexelIndex(size_t pixel_width, int side, int j, int i) const
{
	return pixel_width * (i*side + j);
}

Vect TerrainModel::getFaceNormal(const Vect v1, const Vect v2, const Vect v3)
{
	return ((v2 - v1).cross(v3 - v1)).getNorm();
}

TerrainModel::~TerrainModel()
{
	delete pModTerrain;
}

void TerrainModel::Render()
{
	assert(pTex != nullptr);
	pModTerrain->SetToContext(pShader->GetContext());
	pTex->SetToContext(pShader->GetContext());
	pShader->SendWorldAndMaterial(Matrix(IDENTITY), Colors::White, Colors::White, Vect(0, 0, 0, 100));
	pModTerrain->Render(pShader->GetContext());
}

void TerrainModel::SetTexture(Texture* tex)
{
	pTex = tex;
}
