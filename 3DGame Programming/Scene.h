#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Shader.h"

class CScene
{
public:
	CScene();
	~CScene();

private:
	//int	m_nObjects = 0;		// 오브젝트 개수
	//
	//CGameObject** m_ppObjects = NULL;

	CWallsObject* m_pWallsObject = NULL;

	CPlayer* m_pPlayer = NULL;

protected:
	//배치(Batch) 처리를 하기 위하여 씬을 셰이더들의 리스트로 표현한다.
	CObjectsShader *m_pShaders = NULL;
	
	int m_nShaders = 0;
	
	CShader** m_ppShaders = NULL;

	//루트 시그너쳐를 나타내는 인터페이스 포인터
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;

	//파이프라인 상태를 나타내는 인터페이스 포인터이다.
	ID3D12PipelineState* m_pd3dPipelineState = NULL;

public:
	void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void AnimateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void ReleaseObjects();

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	bool ProcessInput(UCHAR* pKeysBuffer);
	void ReleaseUploadBuffers();

	void SetPlayer(CPlayer* pPlayer, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	CPlayer* GetPlayer() { return m_pPlayer; };

	//그래픽 루트 시그너쳐를 생성한다.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();
};