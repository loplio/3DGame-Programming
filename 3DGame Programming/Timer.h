#pragma once
//-----------------------------------------------------------------------------
// File: CGameTimer.h
//-----------------------------------------------------------------------------

const ULONG MAX_SAMPLE_COUNT = 50;			// frame ��� ��� �ð��� ǥ�� ��

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
	// ���� ����
	double							m_fTimeScale;				// �� �� �ִ� ���� ���� �ð� ���� (0.0000001s)
	float							m_fTimeElapsed;				// 1 frame ��� �ð�
	// �⺻ ����
	__int64							m_nBasePerformanceCounter;	// ���� CPU Ŭ�� �� (���α׷� ����, ���� ����)
	__int64							m_nPausePerformanceCounter;	// ������ ���� CPU Ŭ�� ��
	__int64							m_nStopPerformanceCounter;	// ���� CPU Ŭ�� ��
	// ���� ����
	__int64							m_nCurrentPerformanceCounter;
	__int64							m_nLastPerformanceCounter;
	__int64							m_PerformanceFrequencyPerSec;		// �ʴ� ���� �� (10'000'000)
	float							m_fFrameTime[MAX_SAMPLE_COUNT];		// 1frame ��� �ð��� ���� �迭
	// �⺻ ����
	ULONG							m_nSampleCount;				// frame ��� ��� �ð��� ���� ǥ�� ��

	unsigned long					m_nCurrentFrameRate;		// ���� �ʴ� ������ ��
	unsigned long					m_FramePerSecond;
	float							m_fFPSTimeElapsed;

	bool							m_bStopped;
};