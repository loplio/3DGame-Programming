#pragma once
//-----------------------------------------------------------------------------
// File: CGameTimer.h
//-----------------------------------------------------------------------------

const ULONG MAX_SAMPLE_COUNT = 50;			// frame 평균 경과 시간의 표본 수

class CGameTimer {
public:
	CGameTimer();
	virtual ~CGameTimer() {};

	void Tick(float fLockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	float GetTimeElapsed();
	float GetTotalTime();

private:
	// 대입 변수
	double							m_fTimeScale;				// 셀 수 있는 가장 작은 시간 단위 (0.0000001s)
	float							m_fTimeElapsed;				// 1 frame 경과 시간
	// 기본 변수
	__int64							m_nBasePerformanceCounter;	// 시작 CPU 클럭 수 (프로그램 시작, 게임 시작)
	__int64							m_nPausePerformanceCounter;	// 정지한 동안 CPU 클럭 수
	__int64							m_nStopPerformanceCounter;	// 정지 CPU 클럭 수
	// 대입 변수
	__int64							m_nCurrentPerformanceCounter;
	__int64							m_nLastPerformanceCounter;
	__int64							m_PerformanceFrequencyPerSec;		// 초당 진동 수 (10'000'000)
	float							m_fFrameTime[MAX_SAMPLE_COUNT];		// 1frame 경과 시간에 대한 배열
	// 기본 변수
	ULONG							m_nSampleCount;				// frame 평균 경과 시간의 현재 표본 수

	unsigned long					m_nCurrentFrameRate;		// 현재 초당 프레임 수
	unsigned long					m_FramePerSecond;
	float							m_fFPSTimeElapsed;

	bool							m_bStopped;
};