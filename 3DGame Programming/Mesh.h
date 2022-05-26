#pragma once
#include "stdafx.h"

class CPoint3D {

};

class CVertex
{
protected:
	XMFLOAT3				m_xmf3Position;

public:
	CVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() {}

};

class CPolygon {
public:
	CPolygon() {}
	CPolygon(int nVertices);
	~CPolygon();

	int						m_nVertices = 0;
	CVertex					*m_pVertices = NULL;

	void SetVertex(int nIndex, const CVertex& vertex);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CDiffusedVertex : public CVertex {
protected:
	//������ �����̴�.
	XMFLOAT4 m_xmf4Diffuse;

public:
	CDiffusedVertex() {
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse =
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) {
		m_xmf3Position =
			XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse;
	}
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) {
		m_xmf3Position =
			xmf3Position; m_xmf4Diffuse = xmf4Diffuse;
	}
	~CDiffusedVertex() { }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMesh {
public:
	CMesh() {}
	CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int							m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();

protected:
	ID3D12Resource*				m_pd3dVertexBuffer = NULL;
	ID3D12Resource*				m_pd3dVertexUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW	m_d3dVertexBufferView;
	D3D12_PRIMITIVE_TOPOLOGY	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

protected:
	ID3D12Resource*				m_pd3dIndexBuffer = NULL;
	ID3D12Resource*				m_pd3dIndexUploadBuffer = NULL;
	D3D12_INDEX_BUFFER_VIEW		m_d3dIndexBufferView;
	/*�ε��� ����(�ε����� �迭)�� �ε��� ���۸� ���� ���ε� ���ۿ� ���� �������̽� �������̴�. �ε��� ���۴� ����
	����(�迭)�� ���� �ε����� ������.*/

	UINT m_nIndices = 0;
	//�ε��� ���ۿ� ���ԵǴ� �ε����� �����̴�.
	UINT m_nStartIndex = 0;
	//�ε��� ���ۿ��� �޽��� �׸��� ���� ���Ǵ� ���� �ε����̴�.
	int m_nBaseVertex = 0;
	//�ε��� ������ �ε����� ������ �ε����̴�. 

protected:
	int							m_nPolygons = 0;
	CPolygon					** m_ppPolygons = NULL;

public:
	BoundingOrientedBox			m_xmOOBB = BoundingOrientedBox();

public:
	void SetPolygon(int nIndex, CPolygon* pPolygon);
	virtual void Render(HDC hDCFrameBuffer);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
};

class CTriangleMesh : public CMesh {
public:
	CTriangleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CTriangleMesh() { }
};
//-----------------------------------------------------------------------------------------------------
class CCubeMeshDiffused : public CMesh {
public:
	//������ü�� ����, ����, ������ ���̸� �����Ͽ� ������ü �޽��� �����Ѵ�.
	CCubeMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshDiffused();
};
//-----------------------------------------------------------------------------------------------------
class CCubeMesh : public CMesh {

};
//-----------------------------------------------------------------------------------------------------
class CWallMesh : public CMesh {
public:
	CWallMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, int nSubRects = 20);
	virtual ~CWallMesh() {}
};
//-----------------------------------------------------------------------------------------------------
class CAirplaneMeshDiffused : public CMesh {
public:
	CAirplaneMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f,
		XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMeshDiffused();
};