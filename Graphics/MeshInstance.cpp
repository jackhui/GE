// MeshInstance.cpp

#include "D3D11Renderer.h"
#include "MeshInstance.h"
#include "MeshData.h"

MeshInstance::MeshInstance(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const RenderType renderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture) {
	m_pMeshData = new MeshData(pVertexData, iNumVerts, pIndexData, iNumIndices, renderType, typology, texture);
}

MeshInstance::~MeshInstance() {
	if (m_pMeshData) {
		delete m_pMeshData;
		m_pMeshData = NULL;
	}
}

MeshData* MeshInstance::GetMeshData() {
	return m_pMeshData;
}

void MeshInstance::Transform(const float* scalar, const Vector3* rotation, const Vector3* translation) {
	m_pMeshData->Transform(scalar, rotation, translation);
}

void MeshInstance::Draw()
{
	m_pMeshData->Render();
}