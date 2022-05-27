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
	//int	m_nObjects = 0;		// ������Ʈ ����
	//
	//CGameObject** m_ppObjects = NULL;

	CWallsObject* m_pWallsObject = NULL;

	CPlayer* m_pPlayer = NULL;

protected:
	//��ġ(Batch) ó���� �ϱ� ���Ͽ� ���� ���̴����� ����Ʈ�� ǥ���Ѵ�.
	//CObjectsShader *m_pShaders = NULL;
	CInstancingShader* m_pShaders = NULL;

	int m_nShaders = 0;
	
	CShader** m_ppShaders = NULL;

	//��Ʈ �ñ׳��ĸ� ��Ÿ���� �������̽� ������
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;

	//���������� ���¸� ��Ÿ���� �������̽� �������̴�.
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

	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();
};