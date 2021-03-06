// Alex Niemiec 2020

#ifndef _ShaderTextureLight
#define _ShaderTextureLight

#include "ShaderBase.h"
#include "Matrix.h"
#include "Texture.h"

class ShaderTextureLight : public ShaderBase
{
public:
	ShaderTextureLight(ID3D11Device* device, int sl_count = 0, int pl_count = 0, float fogStart = -1, float fogRange = -1, Vect fogColor = Vect(0, 0, 0, 1));

	ShaderTextureLight(const ShaderTextureLight&) = delete;				// Copy constructor
	ShaderTextureLight(ShaderTextureLight&&) = default;					// Mover constructor
	ShaderTextureLight& operator=(const ShaderTextureLight&) & = default;	// Copy assignment operator
	ShaderTextureLight& operator=(ShaderTextureLight&&) & = default;		// Move assignment
	~ShaderTextureLight();											// Destructor

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters(int id, const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetSpotLightParameters(int id, const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SendWorldAndMaterial(const Matrix& world, const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));

private:

	struct Material
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct PhongADS
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct DirectionalLight
	{
		PhongADS Light;
		Vect Direction;
	};

	DirectionalLight DirLightData;

	struct PointLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		float Range;
	};

	int plCount;
	PointLight* PointLightData;

	struct SpotLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		Vect Direction;
		float SpotExp;
		float Range;
	};

	int slCount;
	SpotLight* SpotLightData;

	// Fog parameters
	float FogStart;
	float FogRange;
	Vect FogColor;

	// constant buffer structs and handles

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer* mpBufferCamMatrices;

	struct WorldMatData
	{
		Matrix World;
		Matrix WorldInv;
		Material Mat;
	};

	ID3D11Buffer* mpBufferWorldMatData;

	struct Data_LightParams
	{
		int SLcount;
		int PLcount;
		DirectionalLight DirLight;
		PointLight PntLight[3];
		SpotLight SptLight[3];
		Vect EyePosWorld;
		// --- Fog Parameters ---
		float FogStart;
		float FogRange;
		Vect FogColor;
	};

	ID3D11Buffer* mpBufferLightParams;

};

#endif _ShaderTextureLight