// MeshData.h: a class contains all pointers to buffer of a mesh

#ifndef MESHDATA_H
#define MESHDATA_H

#include <d3d11.h>
#include "../Object/Camera.h"
#include "VertexFormat.h"

enum RenderType
{
	OUTLINE,
	STANDARD_MESH,
	STANDARD_MESH_WITH_SHADOW,
	V1P,
	V1P1UV,
	V1P1N,
	V1P1N1UV
};

class MeshData
{
public:

	// Overload default constructor
	MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, const RenderType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture);

	void SetUpEnvironment(const RenderType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture);

	void Transform(const float* scalar, const Vector3* rotation, const Vector3* translation);

	void Update();

	void Render();

	inline int GetVertexNum()
	{
		return m_iNumVerts;
	}

	// Destructor
	~MeshData();

private:

	// Pointer to complied to vertex shader
	ID3D11VertexShader*						m_pVS;
	
	// Pointer to complied to pixel shader
	ID3D11PixelShader*						m_pPS;

	// Pointer to vertex buffer
	ID3D11Buffer*							m_pVertexBuffer;

	// Pointer to index buffer
	ID3D11Buffer*							m_pIndexBuffer;

	// Pointer to constant buffer
	ID3D11Buffer*							g_pConstantBuffer;

	// Pointer to input layout supply to IA
	ID3D11InputLayout*						m_pInputLayout;

	// Pointer to resource view pass to shader
	ID3D11ShaderResourceView*				m_pTexResourceView;

	// Pointer to sampler state
	ID3D11SamplerState*						m_pSamplerState;

	// Pointer to blend state
	ID3D11BlendState*						m_pBlendState;

	// Primitive Topology
	unsigned int							m_iTopology;

	// Render Type
	RenderType								m_renderType;

	// Number of vertices
	unsigned int							m_iNumVerts;

	// Number of indics
	unsigned int							m_iNumIndics;

	// Stride
	unsigned int							m_stride;

	//Vertex offset
	unsigned int							m_vertexOffset;
 
	//Start Index Location
	unsigned int							m_iStartIndexLocation;

	Matrix4									TransformationMat;
};

#endif