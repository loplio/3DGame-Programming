#pragma once
#include "Mesh.h"
#include "Camera.h"

class CShader;
class CObjectsShader;

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

struct MATERIALLOADINFO
{
	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;

	UINT							m_nType = 0x00;

	TCHAR							m_pstrAlbedoMapName[128] = { '\0' };
	TCHAR							m_pstrSpecularMapName[128] = { '\0' };
	TCHAR							m_pstrMetallicMapName[128] = { '\0' };
	TCHAR							m_pstrNormalMapName[128] = { '\0' };
	TCHAR							m_pstrEmissionMapName[128] = { '\0' };
	TCHAR							m_pstrDetailAlbedoMapName[128] = { '\0' };
	TCHAR							m_pstrDetailNormalMapName[128] = { '\0' };
};

struct MATERIALSLOADINFO
{
	int								m_nMaterials = 0;
	MATERIALLOADINFO				*m_pMaterials = NULL;
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	CShader* m_pShader = NULL;

	void SetShader(CShader* pShader);
	void SetPseudoLightingShader() { SetShader(m_pPseudoLightingShader); }

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList);

protected:
	static CShader					*m_pPseudoLightingShader;

public:
	static void PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CObjectsShader& ObShader);
};

class CGameObject {
public:
	//CGameObject();
	CGameObject(int nMeshes = 1);
	virtual ~CGameObject();

private:
	DWORD					m_dwColor = RGB(255, 0, 0);

	int						m_nReferences = 0;

protected:
	//게임 객체는 여러 개의 메쉬를 포함하는 경우 게임 객체가 가지는 메쉬들에 대한 포인터와 그 개수이다.
	CMesh					**m_ppMeshes = NULL;
	int						m_nMeshes = 0;
	CMesh*					m_pMesh = NULL;
	CShader*				m_pShader = NULL;

public:
	XMFLOAT4X4				m_xmf4x4World;
	BoundingOrientedBox		m_xmOOBB = BoundingOrientedBox();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetMesh(int nIndex, CMesh* pMesh);
	virtual void SetMesh(CMesh* pMesh);
	virtual void SetShader(CShader* pShader);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT4X4 GetWorld() { return m_xmf4x4World; }

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);
	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Move(float x, float y, float z) { m_xmf4x4World._41 += x; m_xmf4x4World._42 += y; m_xmf4x4World._43 += z; }
	void Move(const XMFLOAT3& mf3Position) { Move(mf3Position.x, mf3Position.y, mf3Position.z); };

	virtual void OnPrepareRender();
	void Render(HDC hDCFrameBuffer,XMFLOAT4X4* pxmf4x4World, CMesh* pMesh);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, const WCHAR* method = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, UINT
		nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, UINT
		nInstances);
	//virtual void Animate(float fElapsedTime);
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

	void UpdateBoundingBox();

	void ReleaseUploadBuffers();

public:
	TCHAR							m_pstrFrameName[256];

	int								m_nMaterials = 0;
	CMaterial						**m_ppMaterials = NULL;

	CGameObject						*m_pParent = NULL;
	CGameObject						*m_pChild = NULL;
	CGameObject						*m_pSibling = NULL;
	
	void SetMaterial(int nMaterial, CMaterial* pMaterial);
	void SetChild(CGameObject* pChild);
	
	virtual void BuildMaterials(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }
	
	CGameObject* GetParent() { return(m_pParent); }
	CGameObject* FindFrame(_TCHAR* pstrFrameName);

	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0); }

	static MATERIALSLOADINFO* LoadMaterialsInfoFromFile(std::wifstream& InFile);
	static CMeshLoadInfo* LoadMeshInfoFromFile(std::wifstream& InFile);

	static CGameObject* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, std::wifstream& InFile);
	static CGameObject* LoadGeometryFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, TCHAR* pstrFileName);
	
	static void PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent);

};

class CRotatingObject : public CGameObject
{
public:
	//CRotatingObject() {};
	CRotatingObject(int nMeshes = 1);
	CRotatingObject(float x, float y, float z, int nMeshes = 1);
	virtual ~CRotatingObject();

private:
	XMFLOAT3 m_xmf3RotationAxis;
	float m_fRotationSpeed;

public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }
	virtual void Animate(float fTimeElapsed);
};


class CWallsObject : public CGameObject{
public:
	CWallsObject();
	virtual ~CWallsObject();

public:
	BoundingOrientedBox			m_xmOOBBPlayerMoveCheck = BoundingOrientedBox();
	XMFLOAT4					m_pxmf4WallPlanes[6];

	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

class CHeightMapTerrain : public CGameObject{
public:
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int
		nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4
		xmf4Color);
	virtual ~CHeightMapTerrain();
private:
	//지형의 높이 맵으로 사용할 이미지이다.
	CHeightMapImage *m_pHeightMapImage;
	//높이 맵의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;
	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다.
	XMFLOAT3 m_xmf3Scale;
public:
	//지형의 높이를 계산하는 함수이다(월드 좌표계). 높이 맵의 높이에 스케일의 y를 곱한 값이다.
	float GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x / 
	m_xmf3Scale.x, z / m_xmf3Scale.z)* m_xmf3Scale.y);
}
//지형의 법선 벡터를 계산하는 함수이다(월드 좌표계). 높이 맵의 법선 벡터를 사용한다.
	XMFLOAT3 GetNormal(float x, float z) { 
return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z /
	m_xmf3Scale.z))); }
	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	//지형의 크기(가로/세로)를 반환한다. 높이 맵의 크기에 스케일을 곱한 값이다.
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
};

class CApacheObject : public CGameObject
{
public:
	CApacheObject();
	virtual ~CApacheObject();

private:
	CGameObject* m_pMainRotorFrame0 = NULL;
	CGameObject* m_pMainRotorFrame1 = NULL;
	CGameObject* m_pTailRotorFrame0 = NULL;
	CGameObject* m_pTailRotorFrame1 = NULL;

public:
	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
};