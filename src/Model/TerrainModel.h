// Alex Niemiec 2020

#ifndef _TerrainModel
#define _TerrainModel

#include "Matrix.h"

class Model;
class Texture;
class ShaderTextureLight;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Align16
{
private:
	Model* pModTerrain;
	Texture* pTex;
	ShaderTextureLight* pShader;

	int TerrainModel::TexelIndex(size_t pixel_width, int side, int j, int i) const;
	Vect getFaceNormal(const Vect v1, const Vect v2, const Vect v3);  // pass Vectors in winding order

public:
	TerrainModel(const TerrainModel&) = delete;				// Copy constructor
	TerrainModel(Model&&) = delete;						// Move constructor
	TerrainModel& operator=(const TerrainModel&) & = delete;  // Copy assignment operator
	TerrainModel& operator=(TerrainModel&&) & = delete;       // Move assignment operator
	~TerrainModel();

	TerrainModel(ID3D11Device* dev, ShaderTextureLight* shader, LPCWSTR heightmapFile, float len = 700, float maxheight = 60, float ytrans = 0, float RepeatU = 30, float RepeatV = 30);

	void Render();
	void SetTexture(Texture* tex);

};

#endif _TerrainModel