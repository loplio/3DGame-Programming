#pragma once
#include "GameObject.h"
#include "Camera.h"

#define DIR_FORWARD 0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT 0x04
#define DIR_RIGHT 0x08
#define DIR_UP 0x10
#define DIR_DOWN 0x20

class CPlayer : public CGameObject
{
public:
	//CPlayer();
	CPlayer(int nMeshes = 1);
	virtual ~CPlayer();

protected:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Gravity;
	float						m_fMaxVelocityXZ;
	float						m_fMaxVelocityY;
	float						m_fFriction = 125.0f;
	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	//플레이어의 위치가 바뀔 때마다 호출되는 OnPlayerUpdateCallback() 함수에서 사용하는 데이터이다.
	LPVOID						m_pPlayerUpdatedContext;
	//카메라의 위치가 바뀔 때마다 호출되는 OnCameraUpdateCallback() 함수에서 사용하는 데이터이다.
	LPVOID						m_pCameraUpdatedContext;
	//플레이어에 현재 설정된 카메라이다.
	CCamera*					m_pCamera = NULL;

public:
	XMFLOAT3					m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float						m_fSpeed = 1.0f;

public:
	XMFLOAT3 GetPosition() { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetGravity(XMFLOAT3&& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	/*플레이어의 위치를 xmf3Position 위치로 설정한다.
	xmf3Position 벡터에서 현재 플레이어의 위치 벡터를 빼면
	현재 플레이어의 위치에서 xmf3Position 방향으로의 벡터가 된다.
	현재 플레이어의 위치에서 이 벡터 만큼 이동한다.*/
	void SetPosition(XMFLOAT3& xmf3Position);
	void SetPosition(XMFLOAT3&& xmf3Position);
	void SetPosition(float x, float y , float z);
	void SetVelocity(XMFLOAT3& xmf3Velocity);

	XMFLOAT3& GetVelocity() { return(m_xmf3Velocity); }
	float GetYaw() { return(m_fYaw); }
	float GetPitch() { return(m_fPitch); }
	float GetRoll() { return(m_fRoll); }

	void Move(DWORD nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed) {}
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void OnCameraUpdateCallback(float fTimeElapsed) {}
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	//카메라를 변경하기 위하여 호출하는 함수이다.
	CCamera* OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed) { return(NULL); }

	//플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다.
	virtual void OnPrepareRender();

	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);

	void Rotate(float x, float y, float z);

	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);

	virtual void OnUpdateTransform();
	//virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void Update(float fTimeElapsed = 0.016f);
	
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return(m_pCamera); };
};
//-----------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------
class CAirplanePlayer : public CPlayer {
public:
	CAirplanePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature, int nMeshes = 1);
	CAirplanePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CAirplanePlayer();

	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
};
//-----------------------------------------------------------------------------------------------------
class CModelPlayer : public CPlayer
{
public:
	CModelPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CModelPlayer();

	CGameObject* m_pMainRotorFrame0 = NULL;
	CGameObject* m_pMainRotorFrame1 = NULL;
	CGameObject* m_pTailRotorFrame0 = NULL;
	CGameObject* m_pTailRotorFrame1 = NULL;

private:
	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

public:
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
};

//-----------------------------------------------------------------------------------------------------
class CTerrainPlayer : public CModelPlayer {
public:
	CTerrainPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext, int nMeshes = 1);
	virtual ~CTerrainPlayer();
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	virtual void OnCameraUpdateCallback(float fTimeElapsed);
};