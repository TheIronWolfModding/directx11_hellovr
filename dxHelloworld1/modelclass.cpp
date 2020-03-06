////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, WCHAR* textureFilename)
{
	bool result;

	isHam = false;
	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, context, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::InitializeHam(ID3D11Device * device, ID3D11DeviceContext * context, WCHAR * textureFilename, vr::HiddenAreaMesh_t ham)
{
		bool result;
	// Initialize the vertex and index buffers.
	std::vector<VertexType2> vertices;
	std::vector<unsigned long> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hr;

	isHam = true;
	
	//float testScale = 1.0f;
	//// Load the vertex array with data.
	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f) * testScale;  // Bottom left.
	//vertices[0].texture = D3DXVECTOR2(0.0, 1.0f);

	//vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f) * testScale;  // Top left.
	//vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	//vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f) * testScale;  // top right.
	//vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

	//vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f) * testScale;  // bottom right.
	//vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);

	//// Load the index array with data.
	//indices[0] = 0;  
	//indices[1] = 1;  
	//indices[2] = 2;  
	//indices[3] = 2;
	//indices[4] = 3;
	//indices[5] = 0;


	//Matrix4 mat;
	//AddCubeToScene(mat, vertices, indices);

	/*float m_fScale = 0.3f;
	float m_iSceneVolumeWidth = 20,
		m_iSceneVolumeHeight = 20,
		m_iSceneVolumeDepth = 20;
	float m_fScaleSpacing = 4.0f;
	Matrix4 matScale;
	matScale.scale(m_fScale, m_fScale, m_fScale);
	Matrix4 matTransform;
	matTransform.translate(
		-((float)m_iSceneVolumeWidth * m_fScaleSpacing) / 2.f,
		-((float)m_iSceneVolumeHeight * m_fScaleSpacing) / 2.f,
		-((float)m_iSceneVolumeDepth * m_fScaleSpacing) / 2.f);

	Matrix4 mat = matScale * matTransform;

	for (int z = 0; z< m_iSceneVolumeDepth; z++)
	{
		for (int y = 0; y< m_iSceneVolumeHeight; y++)
		{
			for (int x = 0; x< m_iSceneVolumeWidth; x++)
			{
				AddCubeToScene(mat, vertices, indices);
				mat = mat * Matrix4().translate(m_fScaleSpacing, 0, 0);
			}
			mat = mat * Matrix4().translate(-((float)m_iSceneVolumeWidth) * m_fScaleSpacing, m_fScaleSpacing, 0);
		}
		mat = mat * Matrix4().translate(0, -((float)m_iSceneVolumeHeight) * m_fScaleSpacing, m_fScaleSpacing);
	}
	
	// TODO_HAM: why?
	for (int i = 0; i < vertices.size() / 2; i++)
	{
		std::swap(vertices[i], vertices[vertices.size() - i - 1]);
	}*/

	for (int tri = 0; tri < ham.unTriangleCount; ++tri)
	{
		
		float z = 0.0f;
		VertexType2 v1;
#define ADJUST
#ifdef ADJUST
		// Get a pointer to the first vertex of the triangle.
		auto const pvd = ham.pVertexData + (tri * 3);
		v1.position.set(((pvd)->v[0] - 0.5f) * 2.0f, ((pvd)->v[1] - 0.5f) * 2.0f, z);
		
		VertexType2 v2;
		v2.position.set(((pvd + 1)->v[0] - 0.5f) * 2.0f, ((pvd + 1)->v[1] - 0.5f) * 2.0f, z);
		
		VertexType2 v3;
		v3.position.set(((pvd + 2)->v[0] - 0.5f) * 2.0f, ((pvd + 2)->v[1] - 0.5f) * 2.0f, z);
#else
		auto const pvd = ham.pVertexData + (tri * 3);
		v1.position.set(((pvd)->v[0]), ((pvd)->v[1]), z);
		
		VertexType2 v2;
		v2.position.set(((pvd + 1)->v[0]), ((pvd + 1)->v[1]), z);
		
		VertexType2 v3;
		v3.position.set(((pvd + 2)->v[0]), ((pvd + 2)->v[1]), z);
#endif 
		
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		indices.push_back((tri * 3));
		indices.push_back((tri * 3) + 1);
		indices.push_back((tri * 3) + 2);
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType2) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	hr = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	m_vertexCount = vertices.size();
	m_indexCount = indices.size();


	// Load the texture for this model.
  result = LoadTexture(device, context, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void ModelClass::AddCubeVertex(float x, float y, float z, float tx, float ty, std::vector<VertexType> &vertdata, D3DXVECTOR3 normal)
{
	VertexType temp;
	temp.position.set(x, y, z);
	temp.texture.set(tx, ty);
	temp.normal = normal;
	vertdata.push_back(temp);
}

void ModelClass::AddCubeToScene(Matrix4 mat, std::vector<VertexType> &vertdata, std::vector<unsigned long> &indices)
{
	// Matrix4 mat( outermat.data() );

	Vector4 A = mat * Vector4(0, 0, 0, 1);
	Vector4 B = mat * Vector4(1, 0, 0, 1);
	Vector4 C = mat * Vector4(1, 1, 0, 1);
	Vector4 D = mat * Vector4(0, 1, 0, 1);
	Vector4 E = mat * Vector4(0, 0, 1, 1);
	Vector4 F = mat * Vector4(1, 0, 1, 1);
	Vector4 G = mat * Vector4(1, 1, 1, 1);
	Vector4 H = mat * Vector4(0, 1, 1, 1);

	int old_vertex_index = vertdata.size();
	// triangles instead of quads
	D3DXVECTOR3 normal;
	normal.set(0, 0, -1);
	AddCubeVertex(E.x, E.y, E.z, 0, 1, vertdata, normal); //Front
	AddCubeVertex(F.x, F.y, F.z, 1, 1, vertdata, normal);
	AddCubeVertex(G.x, G.y, G.z, 1, 0, vertdata, normal);
	AddCubeVertex(G.x, G.y, G.z, 1, 0, vertdata, normal);
	AddCubeVertex(H.x, H.y, H.z, 0, 0, vertdata, normal);
	AddCubeVertex(E.x, E.y, E.z, 0, 1, vertdata, normal);

	normal.set(0, 0, 1);
	AddCubeVertex(B.x, B.y, B.z, 0, 1, vertdata, normal); //Back
	AddCubeVertex(A.x, A.y, A.z, 1, 1, vertdata, normal);
	AddCubeVertex(D.x, D.y, D.z, 1, 0, vertdata, normal);
	AddCubeVertex(D.x, D.y, D.z, 1, 0, vertdata, normal);
	AddCubeVertex(C.x, C.y, C.z, 0, 0, vertdata, normal);
	AddCubeVertex(B.x, B.y, B.z, 0, 1, vertdata, normal);

	normal.set(0, 1, 0);
	AddCubeVertex(H.x, H.y, H.z, 0, 1, vertdata, normal); //Top
	AddCubeVertex(G.x, G.y, G.z, 1, 1, vertdata, normal);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata, normal);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata, normal);
	AddCubeVertex(D.x, D.y, D.z, 0, 0, vertdata, normal);
	AddCubeVertex(H.x, H.y, H.z, 0, 1, vertdata, normal);

	normal.set(0, -1, 0);
	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata, normal); //Bottom
	AddCubeVertex(B.x, B.y, B.z, 1, 1, vertdata, normal);
	AddCubeVertex(F.x, F.y, F.z, 1, 0, vertdata, normal);
	AddCubeVertex(F.x, F.y, F.z, 1, 0, vertdata, normal);
	AddCubeVertex(E.x, E.y, E.z, 0, 0, vertdata, normal);
	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata, normal);

	normal.set(-1, 0, 0);
	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata, normal); //Left
	AddCubeVertex(E.x, E.y, E.z, 1, 1, vertdata, normal);
	AddCubeVertex(H.x, H.y, H.z, 1, 0, vertdata, normal);
	AddCubeVertex(H.x, H.y, H.z, 1, 0, vertdata, normal);
	AddCubeVertex(D.x, D.y, D.z, 0, 0, vertdata, normal);
	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata, normal);

	normal.set(1, 0, 0);
	AddCubeVertex(F.x, F.y, F.z, 0, 1, vertdata, normal); //Right
	AddCubeVertex(B.x, B.y, B.z, 1, 1, vertdata, normal);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata, normal);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata, normal);
	AddCubeVertex(G.x, G.y, G.z, 0, 0, vertdata, normal);
	AddCubeVertex(F.x, F.y, F.z, 0, 1, vertdata, normal);

	int new_vertex_index = vertdata.size();

	for (int i = old_vertex_index; i < new_vertex_index; i++)
	{
		indices.push_back(i);
	}
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	std::vector<VertexType> vertices;
	std::vector<unsigned long> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//float testScale = 1.0f;
	//// Load the vertex array with data.
	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f) * testScale;  // Bottom left.
	//vertices[0].texture = D3DXVECTOR2(0.0, 1.0f);

	//vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f) * testScale;  // Top left.
	//vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	//vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f) * testScale;  // top right.
	//vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

	//vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f) * testScale;  // bottom right.
	//vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);

	//// Load the index array with data.
	//indices[0] = 0;  
	//indices[1] = 1;  
	//indices[2] = 2;  
	//indices[3] = 2;
	//indices[4] = 3;
	//indices[5] = 0;


	//Matrix4 mat;
	//AddCubeToScene(mat, vertices, indices);

	float m_fScale = 0.3f;
	float m_iSceneVolumeWidth = 20,
		m_iSceneVolumeHeight = 20,
		m_iSceneVolumeDepth = 20;
	float m_fScaleSpacing = 4.0f;
	Matrix4 matScale;
	matScale.scale(m_fScale, m_fScale, m_fScale);
	Matrix4 matTransform;
	matTransform.translate(
		-((float)m_iSceneVolumeWidth * m_fScaleSpacing) / 2.f,
		-((float)m_iSceneVolumeHeight * m_fScaleSpacing) / 2.f,
		-((float)m_iSceneVolumeDepth * m_fScaleSpacing) / 2.f);

	Matrix4 mat = matScale * matTransform;

	for (int z = 0; z< m_iSceneVolumeDepth; z++)
	{
		for (int y = 0; y< m_iSceneVolumeHeight; y++)
		{
			for (int x = 0; x< m_iSceneVolumeWidth; x++)
			{
				AddCubeToScene(mat, vertices, indices);
				mat = mat * Matrix4().translate(m_fScaleSpacing, 0, 0);
			}
			mat = mat * Matrix4().translate(-((float)m_iSceneVolumeWidth) * m_fScaleSpacing, m_fScaleSpacing, 0);
		}
		mat = mat * Matrix4().translate(0, -((float)m_iSceneVolumeHeight) * m_fScaleSpacing, m_fScaleSpacing);
	}

	for (int i = 0; i < vertices.size() / 2; i++)
	{
		std::swap(vertices[i], vertices[vertices.size() - i - 1]);
	}
	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	m_vertexCount = vertices.size();
	m_indexCount = indices.size();

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = isHam ? sizeof(VertexType2) : sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* context, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, context, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

