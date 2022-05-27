#pragma once
#include "Player.h"
#include "Scene.h"
#include "Timer.h"

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework() {}

private:
	HINSTANCE				m_hInstance = NULL;
	HWND					m_hWnd = NULL;

	int						m_nWndClientWidth;
	int						m_nWndClientHeight;

	IDXGIFactory4			*m_pdxgiFactory;				// DXGI ���丮 �������̽��� ���� ������.
	IDXGISwapChain3			*m_pdxgiSwapChain;				// ���� ü�� �������̽� ������, ���÷��� ��� �ʿ�
	ID3D12Device			*m_pd3dDevice;					// Direct3D ����̽� �������̽� ������, ���ҽ� ������ �ʿ�

	bool					m_bMsaa4xEnable = false;		// 
	UINT					m_nMsaa4xQualityLevels = 0;		// MSAA ���� ���ø� Ȱ��ȭ �� ���� ���ø� ���� ����

	static const UINT		m_nSwapChainBuffers = 2;		// ���� ü�� �ĸ� ���� ����
	UINT					m_nSwapChainBufferIndex;		// ���� ���� ü�� �ĸ� ���� �ε���

	ID3D12Resource			*m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// ���� Ÿ�� ����
	ID3D12DescriptorHeap	*m_pd3dRtvDescriptorHeap;		// ������ �� �������̽� ������
	UINT					m_nRtvDescriptorIncrementSize;	// ���� Ÿ�� ������ ���� ũ��

	ID3D12Resource			*m_pd3dDepthStencilBuffer;		// ����-���Ľ� ����
	ID3D12DescriptorHeap	*m_pd3dDsvDescriptorHeap;		// ������ �� �������̽� ������
	UINT					m_nDsvDescriptorIncrementSize;	// ����-���Ľ� ������ ���� ũ��

	ID3D12CommandQueue		*m_pd3dCommandQueue;			// ��� ť
	ID3D12CommandAllocator	*m_pd3dCommandAllocator;		// ��� �Ҵ���
	ID3D12GraphicsCommandList	*m_pd3dCommandList;			// ��� ����Ʈ �������̽�

	ID3D12PipelineState		*m_pd3dPipelineState;			// �׷��Ƚ� ���������� ���� ��ü�� ���� �������̽� ������

	ID3D12Fence				*m_pd3dFence;					// �潺 �������̽� ������
	UINT64					m_nFenceValues[m_nSwapChainBuffers];					// �潺�� ��
	HANDLE					m_hFenceEvent;					// �̺�Ʈ �ڵ�

private:
	RECT					m_rcClient;

	HDC						m_hDCFrameBuffer = NULL;
	HBITMAP					m_hBitmapFrameBuffer = NULL;

	CCamera*				m_pCamera = NULL;

	CScene*					m_pScene = NULL;

	CGameTimer				m_GameTimer;

	POINT					m_ptOldCursorPos;

	_TCHAR					m_pszFrameRate[50];

public:
	CPlayer* m_pPlayer = NULL;
	//���������� ���콺 ��ư�� Ŭ���� ���� ���콺 Ŀ���� ��ġ�̴�.
	//POINT m_ptOldCursorPos; 

	bool OnCreate(HINSTANCE hInstance, HWND hWnd);

	void BuildObjects();

	void FrameAdvance();
	void ProcessInput();
	void AnimateObjects();
	void HeapInitAndRendering();
	void MoveToNextFrame();

	void ReleaseObjects();
	void OnDestroy();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CreateDirect3DDevice();					// ������ �� ����
	void CreateCommandQueueAndList();				// ���/ť/�Ҵ���/����Ʈ�� ����
	void CreateRtvAndDsvDescriptorHeaps();			// ����̽� ����
	void CreateRenderTargetViews();					// ���� Ÿ�� �� ����
	void CreateDepthStencilView();					// ����-���ٽ� �� ����
	void CreateSwapChain();							// ���� ü�� ����
	void ChangeSwapChainState();

	void WaitForGpuComplete();						// CPU�� GPU�� ����ȭ�ϴ� �Լ�
};