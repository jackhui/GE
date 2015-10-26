#include "D3D11Renderer.h"
#include "VertexFormat.h"

class VertexBufferEngine
{
public:
	VertexBufferEngine() {};

	ID3D11Buffer* CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize);

	static VertexBufferEngine* GetInstance();
	static void DestructandCleanUp();

private:
	static VertexBufferEngine*		m_pInstance;
};