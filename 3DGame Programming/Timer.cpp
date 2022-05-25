//-----------------------------------------------------------------------------
// File: CGameTimer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Timer.h"

CGameTimer::CGameTimer()
{
	if (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_PerformanceFrequencyPerSec))		// ���ļ��� ���� 1�ʴ� ����Ǵ� ���� ��
	{
		m_bStopped = true;
		::QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastPerformanceCounter);			// ���� CPU Ŭ�� ���� �˷���. �ð� ���� ������ ���
		m_fTimeScale = 1.0 / (double)m_PerformanceFrequencyPerSec;
	}
	else
	{
		m_bStopped = false;
		m_nLastPerformanceCounter = ::timeGetTime();
		m_fTimeScale = 0.001f;
	}


	m_nBasePerformanceCounter = m_nLastPerformanceCounter;
	m_nPausePerformanceCounter = 0;
	m_nStopPerformanceCounter = 0;

	m_nSampleCount = 0;
	m_nCurrentFrameRate = 0;
	m_FramePerSecond = 0;
	m_fFPSTimeElapsed = 0.0f;

	m_bStopped = false;
}

void CGameTimer::Tick(float fLockFPS)
{
	if (m_bStopped)
	{
		m_fTimeElapsed = 0.0f;
		return;
	}
	float fTimeElapsed;				// 1frame ��� �ð�

	::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentPerformanceCounter);
	fTimeElapsed = float((m_nCurrentPerformanceCounter - m_nLastPerformanceCounter) * m_fTimeScale);

	if (fLockFPS > 0.0f)
	{
		while (fTimeElapsed < (1.0f / fLockFPS))
		{
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentPerformanceCounter);
			fTimeElapsed = float((m_nCurrentPerformanceCounter - m_nLastPerformanceCounter) * m_fTimeScale);
		}
	}
	m_nLastPerformanceCounter = m_nCurrentPerformanceCounter;

	if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f)
	{
		::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));		// �ʴ� ������ �迭 ��ĭ �� �̷��
		m_fFrameTime[0] = fTimeElapsed;															// ���� �� �迭�� 1frame ��� �ð� ���� 
		if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
	}

	m_FramePerSecond++;						// ���� ���� ������ (1�� ������ ��)
	m_fFPSTimeElapsed += fTimeElapsed;		// ���� ���� ������ ��� �ð� (1�� ������ ��)
	//std::cout << "���� frame: " << m_FramePerSecond << std::endl;

	if (m_fFPSTimeElapsed > 1.0f)			// �ʴ� ������ �ʰ� �� �ʱ�ȭ
	{
		m_nCurrentFrameRate = m_FramePerSecond;
		m_FramePerSecond = 0;				// �ʴ� ������ �� �ʱ�ȭ
		m_fFPSTimeElapsed = 0.0f;			// �ʴ� ������ ��� �ð� �ʱ�ȭ
	}

	m_fTimeElapsed = 0.0f;					// �� frame ����ð� (Sample Count��ŭ)
	for (ULONG i = 0; i < m_nSampleCount; ++i) m_fTimeElapsed += m_fFrameTime[i];		// �ʴ� ������ ��� �ð�
	if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;							// �ʴ� ������ 1frame ��� ��� �ð�
	//std::cout << "1 frame ��� ��� �ð� (ǥ��=�ֱ� frame 50��): " << m_fTimeElapsed << std::endl;
}

void CGameTimer::Start()
{
	__int64 nPerformanceCounter{};
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);

	if (m_bStopped)
	{
		m_nPausePerformanceCounter += (nPerformanceCounter - m_nStopPerformanceCounter);
		m_nLastPerformanceCounter = nPerformanceCounter;
		m_nStopPerformanceCounter = 0;
		m_bStopped = false;
	}
}

void CGameTimer::Stop()
{
	if (!m_bStopped)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&m_nStopPerformanceCounter);
		m_bStopped = true;
	}
}

void CGameTimer::Reset()
{
	__int64 nPerformanceCounter{};
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);

	m_nBasePerformanceCounter = nPerformanceCounter;
	m_nLastPerformanceCounter = nPerformanceCounter;
	m_nStopPerformanceCounter = 0;
	m_bStopped = false;
}

unsigned long CGameTimer::GetFrameRate(LPTSTR lpszString, int nCharacters)
{
	if (lpszString)
	{
		_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);		// ������ ���ڿ��� ��ȯ (��ȯ ����, ��ȯ ��� ��� ����, ���� ũ��, �� �� �Ǵ� ����)
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));					
	}
	return m_nCurrentFrameRate;
}

float CGameTimer::GetTimeElapsed()
{
	return m_fTimeElapsed;
}

float CGameTimer::GetTotalTime()
{
	if (m_bStopped) return float(((m_nStopPerformanceCounter - m_nPausePerformanceCounter) - m_nBasePerformanceCounter) * m_fTimeScale);
	return float(((m_nCurrentPerformanceCounter - m_nPausePerformanceCounter) - m_nBasePerformanceCounter) * m_fTimeScale);
}