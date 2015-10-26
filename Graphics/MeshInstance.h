// MeshInstance.h: the class for a mesh instance which contains a mesh data

#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H

#include "MeshData.h"
#include "VertexFormat.h"

typedef SIMDVector3 Vector3;
typedef SIMDQuaternion Quaternion;

class MeshInstance
{
public:
	MeshInstance(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const RenderType renderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture = NULL);
	~MeshInstance();

	MeshData* GetMeshData();

	void Draw();
	void Transform(const float* scalar = NULL, const Vector3* rotation = NULL, const Vector3* translation = NULL);

private:
	// Contains all buffer and shaders data
	MeshData*								m_pMeshData;

	// Quaternion
	Quaternion								m_quat;

	// Position vector
	Vector3									m_pos;

};

#endif