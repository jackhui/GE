#include "VertexBufferEngine.h"
#include <stdio.h>
#include "VertexFormat.h"

// #define _CRT_SECURE_NO_WARNINGS

VertexBufferEngine* VertexBufferEngine::m_pInstance;

VertexBufferEngine* VertexBufferEngine::GetInstance() {
	if (!m_pInstance)
		m_pInstance = new VertexBufferEngine();
	return m_pInstance;
}

void VertexBufferEngine::DestructandCleanUp() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

ID3D11Buffer* VertexBufferEngine::CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize)
{
	HRESULT hr;
	ID3D11Buffer* pVertexBuffer;

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = pVertexData;
	vertexResourcesData.SysMemPitch = 0;
	vertexResourcesData.SysMemSlicePitch = 0;

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = iDataSize * iNumVerts;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &pVertexBuffer);
	assert(hr == S_OK);

	return pVertexBuffer;
}