#include "stdafx.h"
#include "Player.h"

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	CGameObject::SetPosition(x, y, z);

	//if (m_pCamera) m_pCamera->SetPosition(x, y, z);
}

void CPlayer::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3CameraOffset = xmf3CameraOffset;
	XMFLOAT3 xmf3 = Vector3::Add(m_xmf3Position, m_xmf3CameraOffset);
	m_pCamera->SetLookAt(xmf3, m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}

void CPlayer::OnUpdateTransform()
{
	//m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (m_pCamera) m_pCamera->Rotate(fPitch, fYaw, fRoll);

	XMFLOAT3 xmf3_AxisY{ 0.0f,1.0f,0.0f };
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3_AxisY), XMConvertToRadians(fYaw));
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	CGameObject::Rotate(&xmf3_AxisY, fYaw);
}

void CPlayer::Animate(float fElapsedTime)
{
	OnUpdateTransform();

	CGameObject::Animate(fElapsedTime);
}

void CPlayer::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, false);
	CGameObject::Move(m_xmf3Velocity);

	//m_pCamera->Update(this, m_xmf3Position, fTimeElapsed);
	m_pCamera->GenerateViewMatrix();


	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(Vector3::ScalarProduct(m_xmf3Velocity, -1.0f));
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fTimeElapsed;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);
}

void CPlayer::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, true);
	}
}

void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(xmf3Shift, m_xmf3Position);
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::Move(float x, float y, float z)
{
	XMFLOAT3 t{ x, y, z };
	Move(t, false);
}

void CPlayer::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	//CGameObject::Render(hDCFrameBuffer, pCamera);
}
void CPlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
RegoPerson::RegoPerson()
{
	m_pCamera = new CCamera();
	m_pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0.0f, 1.0f);		// 뷰포트
	m_pCamera->SetScissorRect(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);					// 씨저 사각형
	m_pCamera->GenerateProjectionMatrix(1.0f, 500.0f, float(FRAMEBUFFER_WIDTH) /			// 투영 변환 행렬
		float(FRAMEBUFFER_HEIGHT), 90.0f);
	m_pCamera->SetLookAt(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 20.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	SetPosition(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z);
}
void RegoPerson::Animate(float fElapsedTime)
{
	if (riding) 
		RideAnimate(fElapsedTime);
	else
		Update(fElapsedTime);
}
void RegoPerson::RideAnimate(float fElapsedTime)
{
	static int n{};
	static float t{};
	if (n < info_n) {
		XMFLOAT3 dvec = course_RC[n];
		++n;
		SetPosition(dvec.x, dvec.y, dvec.z);
		m_pCamera->SetPosition(dvec.x, dvec.y, dvec.z);
		m_pCamera->GenerateViewMatrix();
	}
	else {
		riding = false;
		n = 0;
	}
}