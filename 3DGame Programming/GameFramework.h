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

	IDXGIFactory4			*m_pdxgiFactory;				// DXGI 팩토리 인터페이스에 대한 포인터.
	IDXGISwapChain3			*m_pdxgiSwapChain;				// 스왑 체인 인터페이스 포인터, 디스플레이 제어에 필요
	ID3D12Device			*m_pd3dDevice;					// Direct3D 디바이스 인터페이스 포인터, 리소스 생성에 필요

	bool					m_bMsaa4xEnable = false;		// 
	UINT					m_nMsaa4xQualityLevels = 0;		// MSAA 다중 샘플링 활성화 및 다중 샘플링 레벨 설정

	static const UINT		m_nSwapChainBuffers = 2;		// 스왑 체인 후면 버퍼 개수
	UINT					m_nSwapChainBufferIndex;		// 현재 스왑 체인 후면 버퍼 인덱스

	ID3D12Resource			*m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// 렌더 타겟 버퍼
	ID3D12DescriptorHeap	*m_pd3dRtvDescriptorHeap;		// 서술자 힙 인터페이스 포인터
	UINT					m_nRtvDescriptorIncrementSize;	// 렌더 타겟 서술자 원소 크기

	ID3D12Resource			*m_pd3dDepthStencilBuffer;		// 깊이-스탠실 버퍼
	ID3D12DescriptorHeap	*m_pd3dDsvDescriptorHeap;		// 서술자 힙 인터페이스 포인터
	UINT					m_nDsvDescriptorIncrementSize;	// 깊이-스탠실 서술자 원소 크기

	ID3D12CommandQueue		*m_pd3dCommandQueue;			// 명령 큐
	ID3D12CommandAllocator	*m_pd3dCommandAllocator;		// 명령 할당자
	ID3D12GraphicsCommandList	*m_pd3dCommandList;			// 명령 리스트 인터페이스

	ID3D12PipelineState		*m_pd3dPipelineState;			// 그래픽스 파이프라인 상태 객체에 대한 인터페이스 포인터

	ID3D12Fence				*m_pd3dFence;					// 펜스 인터페이스 포인터
	UINT64					m_nFenceValues[m_nSwapChainBuffers];					// 펜스의 값
	HANDLE					m_hFenceEvent;					// 이벤트 핸들

private:
	RECT					m_rcClient;

	HDC						m_hDCFrameBuffer = NULL;
	HBITMAP					m_hBitmapFrameBuffer = NULL;

	CCamera*				m_pCamera = NULL;

	CPlayer*				m_pPlayer = NULL;

	CScene*					m_pScene = NULL;

	CGameTimer				m_GameTimer;

	POINT					m_ptOldCursorPos;

	_TCHAR					m_pszFrameRate[50];

public:
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

	void CreateDirect3DDevice();					// 서술자 힙 생성
	void CreateCommandQueueAndList();				// 명령/큐/할당자/리스트를 생성
	void CreateRtvAndDsvDescriptorHeaps();			// 디바이스 생성
	void CreateRenderTargetViews();					// 렌더 타겟 뷰 생성
	void CreateDepthStencilView();					// 깊이-스텐실 뷰 생성
	void CreateSwapChain();							// 스왑 체인 생성
	void ChangeSwapChainState();

	void WaitForGpuComplete();						// CPU와 GPU를 동기화하는 함수
};