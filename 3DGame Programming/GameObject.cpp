﻿#include "stdafx.h"
#include "GameObject.h"
#include "GraphicsPipeline.h"
#include "Shader.h"

CShader* CMaterial::m_pPseudoLightingShader = NULL;

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
	if (m_pShader) m_pShader->Release();
}

void CMaterial::SetShader(CShader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CMaterial::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMaterial::PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CObjectsShader& ObShader)
{
	m_pPseudoLightingShader = new CPseudoLightingShader();
	ObShader.CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

	CApacheObject* pApacheObject = new CApacheObject();
	CGameObject* pGameObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, (wchar_t*)L"Models/Apache.txt");
	std::cout << "after pGameObject*: " << pGameObject << std::endl;
	pApacheObject->SetChild(pGameObject);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(-50.0f, 0.0f, 150.0f);
	pApacheObject->SetScale(1.5f, 1.5f, 1.5f);
	pApacheObject->Rotate(0.0f, 90.0f, 0.0f);
	ObShader.m_nObjects = 1;
	ObShader.m_ppObjects = new CGameObject * [ObShader.m_nObjects];
	ObShader.m_ppObjects[0] = pApacheObject;
	std::cout << "obShader.m_ppObjects*: " << ObShader.m_ppObjects[0]->m_pChild << std::endl;
	std::cout << "obShader.m_ppObjects->: " << ObShader.m_ppObjects[0]->m_pChild->m_ppMaterials[0]->m_pShader << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//CGameObject::CGameObject(void)
//{
//	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
//}
CGameObject::CGameObject(int nMeshes)
{
	m_xmf4x4World = Matrix4x4::Identity();
	m_nMeshes = nMeshes;
	m_ppMeshes = NULL;
	if (m_nMeshes > 0)
	{
		m_ppMeshes = new CMesh * [m_nMeshes];
		for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;
	}
}
CGameObject::~CGameObject(void)
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
	//if (m_pMesh) m_pMesh->Release();
	//if (m_pShader)
	//{
	//	m_pShader->ReleaseShaderVariables();
	//	m_pShader->Release();
	//}
}

void CGameObject::SetShader(CShader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}
void CGameObject::SetMesh(CMesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}
void CGameObject::SetMesh(int nIndex, CMesh* pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void CGameObject::SetMaterial(int nMaterial, CMaterial* pMaterial)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->Release();
	m_ppMaterials[nMaterial] = pMaterial;
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->AddRef();
}
void CGameObject::SetChild(CGameObject* pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	if (pChild) pChild->m_pParent = this;
}

void CGameObject::CreateShaderVariables(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList)
{
}
void CGameObject::ReleaseShaderVariables()
{
}
void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
	//객체의 월드 변환 행렬을 루트 상수(32-비트 값)를 통하여 셰이더 변수(상수 버퍼)로 복사한다.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}
void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}
void CGameObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4World = Matrix4x4::Multiply(mtxScale, m_xmf4x4World);

	UpdateTransform(NULL);
}

void CGameObject::UpdateTransform(XMFLOAT4X4* pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4World, *pxmf4x4Parent) : m_xmf4x4World;

	if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}
//게임 객체의 로컬 z-축 벡터를 반환한다.
XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32,
		m_xmf4x4World._33)));
}
//게임 객체의 로컬 y-축 벡터를 반환한다.
XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22,
		m_xmf4x4World._23)));
}
//게임 객체의 로컬 x-축 벡터를 반환한다.
XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12,
		m_xmf4x4World._13)));
}
//게임 객체를 로컬 x-축 방향으로 이동한다.
void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//게임 객체를 로컬 y-축 방향으로 이동한다.
void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//게임 객체를 로컬 z-축 방향으로 이동한다.
void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//게임 객체를 주어진 각도로 회전한다.
void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch),
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::UpdateBoundingBox()
{

}

void CGameObject::Rotate(XMFLOAT3* pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis),
		XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

//void CGameObject::Animate(float fElapsedTime)		// DX12 라이브러리 필요
//{
//	//if (m_fRotationSpeed != 0.0f) Rotate(1.0f,1.0f,1.0f);			// 상황따라
//	//if (m_fMovingSpeed != 0.0f) Move(1.0f,1.0f,1.0f);
//
//	//UpdateBoundingBox();
//}

void CGameObject::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	if (m_pSibling) m_pSibling->Animate(fTimeElapsed, pxmf4x4Parent);
	if (m_pChild) m_pChild->Animate(fTimeElapsed, &m_xmf4x4World);
}

void CGameObject::OnPrepareRender()
{
}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, const WCHAR* Method)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);
	if (Method == NULL) {
		if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
		//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
		if (m_ppMeshes)
		{
			for (int i = 0; i < m_nMeshes; i++)
			{
				if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList);
			}
		}
	}
	else {
		if (m_nMaterials > 0)
		{
			for (int i = 0; i < m_nMaterials; i++)
			{
				if (m_ppMaterials[i])
				{
					if (m_ppMaterials[i]->m_pShader) m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
					m_ppMaterials[i]->UpdateShaderVariable(pd3dCommandList);
				}

				//if (m_pMesh) m_pMesh->Render(pd3dCommandList, i);
			}
		}
		if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera, L"Model");
		if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera, L"Model");
	}
	//OnPrepareRender();
	////객체의 정보를 셰이더 변수(상수 버퍼)로 복사한다.
	//UpdateShaderVariables(pd3dCommandList);
	//if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
	//if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender();
	if (m_pShader) m_pShader->Render(pd3dCommandList);
	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}

//void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
//{
//	OnPrepareRender();
//	if (m_pShader)
//	{
//		//게임 객체의 월드 변환 행렬을 셰이더의 상수 버퍼로 전달(복사)한다.
//		m_pShader->UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
//		m_pShader->Render(pd3dCommandList, pCamera);
//	}
//	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
//}

void CGameObject::Render(HDC hDCFrameBuffer, XMFLOAT4X4* pxmf4x4World, CMesh* pMesh)
{
	if (pMesh)
	{
		CGraphicsPipeline::SetWorldTrasform(pxmf4x4World);			// 월드 변환.

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		pMesh->Render(hDCFrameBuffer);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (pCamera->IsInFrustum(m_xmOOBB)) CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
}
//인스턴싱 정점 버퍼 뷰를 사용하여 메쉬를 렌더링한다.
void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera,
	UINT nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView)
{
	OnPrepareRender();
	if (m_pMesh) m_pMesh->Render(pd3dCommandList, nInstances, d3dInstancingBufferView);
}
void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera,
	UINT nInstances)
{
	OnPrepareRender();
	if (m_pMesh) m_pMesh->Render(pd3dCommandList, nInstances);
}
void CGameObject::ReleaseUploadBuffers()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->ReleaseUploadBuffers();
		}
	}
	////정점 버퍼를 위한 업로드 버퍼를 소멸시킨다.
	//if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
}

CGameObject* CGameObject::FindFrame(_TCHAR* pstrFrameName)
{
	CGameObject* pFrameObject = NULL;
	if (!_tcsncmp(m_pstrFrameName, pstrFrameName, _tcslen(pstrFrameName))) return(this);

	if (m_pSibling) if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) return(pFrameObject);
	if (m_pChild) if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) return(pFrameObject);

	return(NULL);
}
CGameObject* CGameObject::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, std::wifstream& InFile)
{
	TCHAR pstrToken[256] = { 0 };

	int nFrame = 0;

	CGameObject* pGameObject = NULL;
	for (; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Frame>:")))
		{
			pGameObject = new CGameObject();

			InFile >> nFrame;
			InFile >> pGameObject->m_pstrFrameName;
		}
		else if (!_tcscmp(pstrToken, _T("<Transform>:")))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			InFile >> xmf3Position.x >> xmf3Position.y >> xmf3Position.z;
			InFile >> xmf3Rotation.x >> xmf3Rotation.y >> xmf3Rotation.z; //Euler Angle
			InFile >> xmf3Scale.x >> xmf3Scale.y >> xmf3Scale.z;
			InFile >> xmf4Rotation.x >> xmf4Rotation.y >> xmf4Rotation.z >> xmf4Rotation.w; //Quaternion
		}
		else if (!_tcscmp(pstrToken, _T("<TransformMatrix>:")))
		{
			InFile >> pGameObject->m_xmf4x4World._11 >> pGameObject->m_xmf4x4World._12 >> pGameObject->m_xmf4x4World._13 >> pGameObject->m_xmf4x4World._14; //ToParentTransform
			InFile >> pGameObject->m_xmf4x4World._21 >> pGameObject->m_xmf4x4World._22 >> pGameObject->m_xmf4x4World._23 >> pGameObject->m_xmf4x4World._24;
			InFile >> pGameObject->m_xmf4x4World._31 >> pGameObject->m_xmf4x4World._32 >> pGameObject->m_xmf4x4World._33 >> pGameObject->m_xmf4x4World._34;
			InFile >> pGameObject->m_xmf4x4World._41 >> pGameObject->m_xmf4x4World._42 >> pGameObject->m_xmf4x4World._43 >> pGameObject->m_xmf4x4World._44;
		}
		else if (!_tcscmp(pstrToken, _T("<Mesh>:")))
		{
			CMeshLoadInfo* pMeshInfo = pGameObject->LoadMeshInfoFromFile(InFile);
			if (pMeshInfo)
			{
				CMesh* pMesh = NULL;
				if (pMeshInfo->m_nType & VERTEXT_NORMAL)
				{
					pMesh = new CMeshIlluminatedFromFile(pd3dDevice, pd3dCommandList, pMeshInfo);
				}
				if (pMesh) pGameObject->SetMesh(pMesh);
				delete pMeshInfo;
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Materials>:")))
		{
			MATERIALSLOADINFO* pMaterialsInfo = pGameObject->LoadMaterialsInfoFromFile(InFile);
			std::cout << pMaterialsInfo->m_nMaterials << std::endl;
			if (pMaterialsInfo && (pMaterialsInfo->m_nMaterials > 0))
			{
				pGameObject->m_nMaterials = pMaterialsInfo->m_nMaterials;
				pGameObject->m_ppMaterials = new CMaterial * [pMaterialsInfo->m_nMaterials];

				for (int i = 0; i < pMaterialsInfo->m_nMaterials; i++)
				{
					pGameObject->m_ppMaterials[i] = NULL;

					CMaterial* pMaterial = new CMaterial();
					if (pGameObject->GetMeshType() & VERTEXT_NORMAL) pMaterial->SetPseudoLightingShader();
					std::cout << pMaterial->m_pShader << std::endl;

					pGameObject->SetMaterial(i, pMaterial);
					std::cout << "obj->shader : " << pGameObject->m_ppMaterials[0]->m_pShader << std::endl;
					std::cout << "obj* : " << pGameObject << std::endl;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Children>:")))
		{
			int nChilds = 0;
			InFile >> nChilds;

			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					CGameObject* pChild = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, InFile);
					if (pChild) pGameObject->SetChild(pChild);
#ifdef _WITH_DEBUG_RUNTIME_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, _T("(Child Frame: %p) (Parent Frame: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("</Frame>")))
		{
			break;
		}
	}
	return(pGameObject);
}
#define _WITH_DEBUG_FRAME_HIERARCHY
CGameObject* CGameObject::LoadGeometryFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, TCHAR* pstrFileName)
{
	CGameObject* pGameObject = NULL;
	std::wifstream InFile(pstrFileName);
	
	TCHAR pstrToken[256] = { 0 };

	for (; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Hierarchy>:")))
		{
			pGameObject = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, InFile);
			std::cout << "return 전: " << pGameObject << std::endl;
		}
		else if (!_tcscmp(pstrToken, _T("</Hierarchy>")))
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("\nFrame Hierarchy(%s)\n"), pstrFileName);
	for (int i{}; i < 256; ++i)
		std::cout << (char)pstrDebug[i];
	std::cout << std::endl;
	OutputDebugString(pstrDebug);

	CGameObject::PrintFrameInfo(pGameObject, NULL);
#endif
	return(pGameObject);
}

void CGameObject::PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);
	for (int i{}; i < 256; ++i)
		std::cout << (char)pstrDebug[i];
	std::cout << std::endl;
	if (pGameObject->m_pSibling) CGameObject::PrintFrameInfo(pGameObject->m_pSibling, pParent);
	if (pGameObject->m_pChild) CGameObject::PrintFrameInfo(pGameObject->m_pChild, pGameObject);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRotatingObject::CRotatingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 15.0f;
}
CRotatingObject::CRotatingObject(float x, float y, float z, int nMeshes) : CGameObject(nMeshes)
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.0f;
	SetPosition(x, y, z);
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallsObject::CWallsObject()
{

}

CWallsObject::~CWallsObject()
{

}

void CWallsObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CHeightMapTerrain::CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int
	nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4
	xmf4Color) : CGameObject(0)
{
	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
	m_nWidth = nWidth;
	m_nLength = nLength;
	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크
	기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;
	//xmf3Scale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다.
	m_xmf3Scale = xmf3Scale;
	//지형에 사용할 높이 맵을 생성한다.
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);
	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다.
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
	m_nMeshes = cxBlocks * czBlocks;
	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.
	m_ppMeshes = new CMesh * [m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)m_ppMeshes[i] = NULL;
	CHeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치(좌표)이다.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart,
				zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z * cxBlocks), pHeightMapGridMesh);
		}
	}
	//지형을 렌더링하기 위한 셰이더를 생성한다.
	CTerrainShader* pShader = new CTerrainShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	SetShader(pShader);
}
CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CMeshLoadInfo* CGameObject::LoadMeshInfoFromFile(std::wifstream& InFile)
{
	TCHAR pstrToken[256] = { 0 };
	int nPositions = 0, nColors = 0, nNormals = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	CMeshLoadInfo* pMeshInfo = new CMeshLoadInfo;

	InFile >> pMeshInfo->m_nVertices;
	InFile >> pMeshInfo->m_pstrMeshName;

	for (; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Bounds>:")))
		{
			InFile >> pMeshInfo->m_xmf3AABBCenter.x >> pMeshInfo->m_xmf3AABBCenter.y >> pMeshInfo->m_xmf3AABBCenter.z >> pMeshInfo->m_xmf3AABBExtents.x >> pMeshInfo->m_xmf3AABBExtents.y >> pMeshInfo->m_xmf3AABBExtents.z;
		}
		else if (!_tcscmp(pstrToken, _T("<Positions>:")))
		{
			InFile >> nPositions;
			if (nPositions > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_POSITION;
				pMeshInfo->m_pxmf3Positions = new XMFLOAT3[nPositions];
				for (int i = 0; i < nPositions; i++)
				{
					InFile >> pMeshInfo->m_pxmf3Positions[i].x >> pMeshInfo->m_pxmf3Positions[i].y >> pMeshInfo->m_pxmf3Positions[i].z;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Colors>:")))
		{
			InFile >> nColors;
			if (nColors > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_COLOR;
				pMeshInfo->m_pxmf4Colors = new XMFLOAT4[nColors];
				for (int i = 0; i < nColors; i++)
				{
					InFile >> pMeshInfo->m_pxmf4Colors[i].x >> pMeshInfo->m_pxmf4Colors[i].y >> pMeshInfo->m_pxmf4Colors[i].z >> pMeshInfo->m_pxmf4Colors[i].w;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Normals>:")))
		{
			InFile >> nNormals;
			if (nNormals > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_NORMAL;
				pMeshInfo->m_pxmf3Normals = new XMFLOAT3[nNormals];
				for (int i = 0; i < nNormals; i++)
				{
					InFile >> pMeshInfo->m_pxmf3Normals[i].x >> pMeshInfo->m_pxmf3Normals[i].y >> pMeshInfo->m_pxmf3Normals[i].z;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Indices>:")))
		{
			InFile >> nIndices;
			if (nIndices > 0)
			{
				pMeshInfo->m_pnIndices = new UINT[nIndices];
				for (int i = 0; i < nIndices; i++) InFile >> pMeshInfo->m_pnIndices[i];
			}
		}
		else if (!_tcscmp(pstrToken, _T("<SubMeshes>:")))
		{
			InFile >> pMeshInfo->m_nSubMeshes;
			if (pMeshInfo->m_nSubMeshes > 0)
			{
				pMeshInfo->m_pnSubSetIndices = new int[pMeshInfo->m_nSubMeshes];
				pMeshInfo->m_ppnSubSetIndices = new UINT * [pMeshInfo->m_nSubMeshes];
				for (int i = 0; i < pMeshInfo->m_nSubMeshes; i++)
				{
					InFile >> pstrToken;
					if (!_tcscmp(pstrToken, _T("<SubMesh>:")))
					{
						InFile >> pstrToken; //i
						InFile >> pMeshInfo->m_pnSubSetIndices[i];
						if (pMeshInfo->m_pnSubSetIndices[i] > 0)
						{
							pMeshInfo->m_ppnSubSetIndices[i] = new UINT[pMeshInfo->m_pnSubSetIndices[i]];
							for (int j = 0; j < pMeshInfo->m_pnSubSetIndices[i]; j++) InFile >> pMeshInfo->m_ppnSubSetIndices[i][j];
						}

					}
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("</Mesh>")))
		{
			break;
		}
	}
	return(pMeshInfo);
}


MATERIALSLOADINFO* CGameObject::LoadMaterialsInfoFromFile(std::wifstream& InFile)
{
	TCHAR pstrToken[256] = { 0 };
	int nMaterial = 0;

	MATERIALSLOADINFO* pMaterialsInfo = new MATERIALSLOADINFO;

	InFile >> pMaterialsInfo->m_nMaterials;
	pMaterialsInfo->m_pMaterials = new MATERIALLOADINFO[pMaterialsInfo->m_nMaterials];

	for (; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Material>:")))
		{
			InFile >> nMaterial;
		}
		else if (!_tcscmp(pstrToken, _T("<AlbedoColor>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.x >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.y >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.z >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.w;
		}
		else if (!_tcscmp(pstrToken, _T("<EmissiveColor>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.x >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.y >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.z >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.w;
		}
		else if (!_tcscmp(pstrToken, _T("<SpecularColor>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.x >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.y >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.z >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.w;
		}
		else if (!_tcscmp(pstrToken, _T("<Glossiness>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossiness;
		}
		else if (!_tcscmp(pstrToken, _T("<Smoothness>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fSmoothness;
		}
		else if (!_tcscmp(pstrToken, _T("<Metallic>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fSpecularHighlight;
		}
		else if (!_tcscmp(pstrToken, _T("<SpecularHighlight>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fMetallic;
		}
		else if (!_tcscmp(pstrToken, _T("<GlossyReflection>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossyReflection;
		}
		else if (!_tcscmp(pstrToken, _T("</Materials>")))
		{
			break;
		}
	}
	return(pMaterialsInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CApacheObject::CApacheObject()
{
}

CApacheObject::~CApacheObject()
{
}

void CApacheObject::OnInitialize()
{
	m_pMainRotorFrame0 = FindFrame((wchar_t*)L"rotor");
	//	m_pMainRotorFrame1 = FindFrame(L"black_m_6");
	m_pTailRotorFrame0 = FindFrame((wchar_t*)L"black_m_7");
	//	m_pTailRotorFrame1 = FindFrame(L"black_m_8");
}

void CApacheObject::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	if (m_pMainRotorFrame0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 2.0f) * fTimeElapsed);
		m_pMainRotorFrame0->m_xmf4x4World = Matrix4x4::Multiply(xmmtxRotate, m_pMainRotorFrame0->m_xmf4x4World);
	}
	if (m_pTailRotorFrame0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 4.0f) * fTimeElapsed);
		m_pTailRotorFrame0->m_xmf4x4World = Matrix4x4::Multiply(xmmtxRotate, m_pTailRotorFrame0->m_xmf4x4World);
	}

	CGameObject::Animate(fTimeElapsed, pxmf4x4Parent);
}
