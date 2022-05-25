#pragma once
#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer() {};
	virtual ~CPlayer();

protected:
	CCamera*					m_pCamera = NULL;

public:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;
	float						m_fSpeed = 1.0f;

public:
	void SetPosition(float x, float y , float z);

	void Move(DWORD dwDirection, float fDistance);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);

	void Rotate(float x, float y, float z);

	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void Update(float fTimeElapsed = 0.016f);
	
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	virtual CCamera* GetCamera() { return(m_pCamera); };
};

class RegoPerson : public CPlayer{
public:
	bool riding = false;
	XMFLOAT3* course_RC;
	UINT info_n;

public:
	RegoPerson();
	~RegoPerson() { if (course_RC) delete course_RC; }
	virtual void Animate(float fElapsedTime);
	void RideAnimate(float fElapsedTime);
};