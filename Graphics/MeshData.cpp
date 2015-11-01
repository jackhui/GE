// MeshData.cpp

#include "MeshData.h"
#include "..\Math\simdmath.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "VertexBufferEngine.h"
#include "IndexBufferEngine.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <float.h>
#pragma comment (lib, "D3DCompiler")

typedef SIMDVector3 Vector3;

struct VS_CONSTANT_BUFFER
{
	Matrix4 CameraMat;
	Matrix4 ProjectionMat;
	Matrix4 TransformationMat;
};

MeshData::MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, const RenderType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture) :
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	TransformationMat(Matrix4::Identity)
{
	SetUpEnvironment(renderType, typology, texture);

	switch (renderType) {
	case RenderType::V1P:
		m_stride = sizeof(Vertex1P);
		break;
	case RenderType::V1P1N:
		m_stride = sizeof(Vertex1P1N);
		break;
	case RenderType::V1P1N1UV:
		m_stride = sizeof(Vertex1P1N1UV);
		break;
	case RenderType::V1P1UV:
		m_stride = sizeof(Vertex1P1UV);
		break;
	}

	// Create vertex buffer
	m_pVertexBuffer = VertexBufferEngine::GetInstance()->CreateBufferFromRawData(pVertexData, iNumVerts, m_stride);
	// Create index buffer
	m_iStartIndexLocation = IndexBufferEngine::GetCurrentIndex();
	m_pIndexBuffer = IndexBufferEngine::GetInstance()->CreateBufferFromRawData(pIndexData, iNumIndics);
	m_iNumIndics = IndexBufferEngine::GetCurrentIndex() - m_iStartIndexLocation;
}

MeshData::~MeshData() {
	if (m_pVS)
		m_pVS->Release();
	if (m_pPS)
		m_pPS->Release();
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();
	if (g_pConstantBuffer)
		g_pConstantBuffer->Release();
	if (m_pInputLayout)
		m_pInputLayout->Release();
	if (m_pTexResourceView)
		m_pTexResourceView->Release();
	if (m_pSamplerState)
		m_pSamplerState->Release();
	if (m_pBlendState)
		m_pBlendState->Release();

	TextureManager::GetInstance()->DestructandCleanUp();
	ShaderManager::GetInstance()->DestructandCleanUp();
	VertexBufferEngine::GetInstance()->DestructandCleanUp();
	IndexBufferEngine::GetInstance()->DestructandCleanUp();
}

void MeshData::SetUpEnvironment(const RenderType renderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture) {
	m_renderType = renderType;			// Set render type
	m_vertexOffset = 0;					// Set vertex offset
	m_iTopology = typology; 			// Set primitive topology

	switch (renderType)
	{
		case RenderType::V1P1UV:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P1UV.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set texture resources view
			m_pTexResourceView = (ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(texture);
			// Set shader sampler state
			m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
			m_pBlendState = (ID3D11BlendState*) TextureManager::GetInstance()->GetBlendState();
			break;

		case RenderType::V1P1N1UV:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P1N1UV.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P1UV.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/texture.hlsl", D3D11_SHVER_PIXEL_SHADER);
			// Set texture resources view
			m_pTexResourceView = (ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(texture);
			// Set shader sampler state
			m_pSamplerState = (ID3D11SamplerState*)TextureManager::GetInstance()->GetSamplerState(eSamplerState::LINEAR_MIPMAP_MAX_LOD);
			m_pBlendState = (ID3D11BlendState*)TextureManager::GetInstance()->GetBlendState();
			break;

		case RenderType::V1P1N:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P1N.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P1N.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/red.hlsl", D3D11_SHVER_PIXEL_SHADER);
			break;

		case RenderType::V1P:
			// Set vertex shader
			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader("Shaders/vertex1P.hlsl", D3D11_SHVER_VERTEX_SHADER);
			// Set input layout
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout("Shaders/vertex1P.hlsl");
			// Set pixel shader
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader("Shaders/red.hlsl", D3D11_SHVER_PIXEL_SHADER);
			break;
	}

	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;

	VS_CONSTANT_BUFFER vsConstData;
	vsConstData.CameraMat = D3D11Renderer::GetInstance()->GetCamera()->getViewMatrix();
	vsConstData.ProjectionMat.CreatePerspectiveFOV(
		0.785398163f, 
		1042.0f / 768.0f, 
		1.0f, 1000.0f
	);
	vsConstData.ProjectionMat.Multiply(vsConstData.CameraMat);
	Matrix4 rotation;
	rotation.CreateRotationY(PI); // 180 degree
	vsConstData.ProjectionMat.Multiply(rotation);
	vsConstData.TransformationMat = Matrix4::Identity;

	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	HRESULT hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &g_pConstantBuffer);
	assert(hr == S_OK);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
}

void MeshData::Transform(const float* scalar, const Vector3* rotation, const Vector3* translation) {
	Matrix4 Transform;
	Vector3 objectOrigin(0.0f, 0.0f, 0.0f);
	objectOrigin.TransformAsVector(TransformationMat);

	if (scalar) {
		Transform.CreateScale(*scalar);
		TransformationMat.Multiply(Transform);
	}
	if (rotation) {
		bool is_rotation_about_objectOrigin = fabs(rotation->GetW() - 1.0f) < 0.005f;

		if (is_rotation_about_objectOrigin) {
			Vector3 translationOrigin(-objectOrigin.GetX(), -objectOrigin.GetY(), -objectOrigin.GetZ());
			Transform.CreateTranslation(translationOrigin);
			TransformationMat.Multiply(Transform);

			Transform.CreateRotationX(rotation->GetX());
			TransformationMat.Multiply(Transform);
			Transform.CreateRotationY(rotation->GetY());
			TransformationMat.Multiply(Transform);
			Transform.CreateRotationZ(rotation->GetZ());
			TransformationMat.Multiply(Transform);

			Transform.CreateTranslation(objectOrigin);
			TransformationMat.Multiply(Transform);
		} else {
			Transform.CreateRotationX(rotation->GetX());
			TransformationMat.Multiply(Transform);
			Transform.CreateRotationY(rotation->GetY());
			TransformationMat.Multiply(Transform);
			Transform.CreateRotationZ(rotation->GetZ());
			TransformationMat.Multiply(Transform);
		}
	}
	if (translation) {
		Transform.CreateTranslation(*translation);	
		Transform.Multiply(TransformationMat);
		TransformationMat = Transform;
	}

	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(
		0.785398163f,
		1042.0f / 768.0f,
		1.0f,
		1000.0f
	);
	ProjectionMat.Multiply(D3D11Renderer::GetInstance()->GetCamera()->getViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	pCBuffer->ProjectionMat = ProjectionMat;
	pCBuffer->TransformationMat = TransformationMat;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(g_pConstantBuffer, 0);
}

void MeshData::Update()
{
	// Set constant buffer description
	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(
		0.785398163f,
		1042.0f / 768.0f, 
		1.0f, 
		1000.0f
	);
	ProjectionMat.Multiply(D3D11Renderer::GetInstance()->GetCamera()->getViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	pCBuffer->ProjectionMat = ProjectionMat;
	pCBuffer->TransformationMat = TransformationMat;
	D3D11Renderer::GetInstance()->m_pD3D11Context->Unmap(g_pConstantBuffer, 0);
}

void MeshData::Render()
{
	Update();

	// Binding
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_vertexOffset);
	D3D11Renderer::GetInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D11Renderer::GetInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	if (m_pTexResourceView != NULL) {
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, 1, &m_pTexResourceView);
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetSamplers(0, 1, &m_pSamplerState);
		D3D11Renderer::GetInstance()->m_pD3D11Context->OMSetBlendState(m_pBlendState, NULL, 0xFFFFFFFF);
	}

	D3D11Renderer::GetInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
	
}