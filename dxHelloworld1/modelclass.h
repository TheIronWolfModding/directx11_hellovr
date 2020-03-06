////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vectors.h"
#include "Matrices.h"
#include <vector>
#include "../openvr/headers/openvr.h"

typedef Vector3 D3DXVECTOR3;
//typedef DirectX::XMFLOAT4 D3DXVECTOR4;
typedef Vector2 D3DXVECTOR2;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;

		VertexType() {}
	};

	struct VertexType2
	{
		D3DXVECTOR3 position;
		VertexType2() {}
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	bool InitializeHam(ID3D11Device*, ID3D11DeviceContext*, WCHAR*, vr::HiddenAreaMesh_t);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();

	void AddCubeVertex(float x, float y, float z, float tx, float ty, std::vector<VertexType> &vertdata, D3DXVECTOR3 normal);
	void ModelClass::AddCubeToScene(Matrix4 mat, std::vector<VertexType> &vertdata, std::vector<unsigned long> &indices);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	bool isHam = false;
};

#endif