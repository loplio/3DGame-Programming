#pragma once
#include "Mesh.h"

#define ASPECT_RATIO (float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;
};

class CViewport
{
public:
	CViewport(int nLeft, int nTop, int nWidth, int nHeight) { m_nLeft = nLeft, m_nTop = nTop, m_nWidth = nWidth, m_nHeight = nHeight; }
	virtual ~CViewport() {}

	int							m_nLeft;
	int							m_nTop;
	int							m_nWidth;
	int							m_nHeight;
};

class CPlayer;

class CCamera {
public:
	CCamera();
	virtual ~CCamera();

private:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMFLOAT3					m_xmfDistance = XMFLOAT3(0.0f, 20.0f, -50.0f);

	BoundingFrustum				m_xmFrustumView = BoundingFrustum();
	BoundingFrustum				m_xmFrustumWorld = BoundingFrustum();
	XMFLOAT4X4					m_xmf4x4InverseView = Matrix4x4::Identity();

	float						m_fFOVAngle = 90.0f;
	float						m_fProjectRectDistance = 1.0f;

	float						m_fAspectRatio = float(FRAMEBUFFER_WIDTH) / float(FRAMEBUFFER_HEIGHT);

protected:
	//카메라 변환 행렬
	XMFLOAT4X4					m_xmf4x4View;
	//투영 변환 행렬
	XMFLOAT4X4					m_xmf4x4Projection;
	//뷰포트와 씨저 사각형
	D3D12_VIEWPORT				m_d3dViewport;
	D3D12_RECT					m_d3dScissorRect;

public:
	XMFLOAT4X4					m_xmf4x4PerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4					m_xmf4x4ViewPerspectiveProject = Matrix4x4::Identity();

	XMFLOAT4X4					m_xmf4x4OrthographicProject = Matrix4x4::Identity();
	XMFLOAT4X4					m_xmf4x4ViewOrthographicsProject = Matrix4x4::Identity();

	CViewport*					m_pViewport = NULL;

public:
	void GenerateViewMatrix();
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void GenerateOrthographicProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fWidth, float hHeight);

	void SetFOVAngle(float fFOVAngle);

	void SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3& vPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3&& vPosition, XMFLOAT3&& xmf3LookAt, XMFLOAT3&& xmf3Up);
	void SetPosition(float x, float y, float z);

	void Move(XMFLOAT3& xmf3Shift);
	void Move(XMFLOAT3&& xmf3Shift);
	void Move(float x, float y, float z);

	void Rotate(float fPitch, float fYaw, float fRoll);

	void Update(CPlayer* pPlayer, XMFLOAT3& xmfLookAt, float fTimeElapsed = 0.016f);

	bool IsInFrustum(BoundingOrientedBox& xmBoundingBox);


public:
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);
};