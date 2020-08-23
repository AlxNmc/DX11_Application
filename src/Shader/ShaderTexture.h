// Alex Niemiec 2020


#ifndef _ShaderTexture
#define _ShaderTexture

#include "ShaderBase.h"
#include "Matrix.h"
#include "Texture.h"

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(ID3D11Device* device);

	ShaderTexture(const ShaderTexture&) = delete;				// Copy constructor
	ShaderTexture(ShaderTexture&&) = default;					// Mover constructor
	ShaderTexture& operator=(const ShaderTexture&) & = default;	// Copy assignment operator
	ShaderTexture& operator=(ShaderTexture&&) & = default;		// Move assignment
	~ShaderTexture();											// Destructor

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;
	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void ShaderTexture::SendWorld(const Matrix& world);

private:
	// constant buffer structs and handles

	struct CamData 
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer* mpBufferCamData;

	struct WorldData 
	{
		Matrix World;
	};

	ID3D11Buffer* mpBufferWorldData;

};

#endif _ShaderTexture