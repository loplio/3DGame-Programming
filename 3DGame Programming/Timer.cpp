//-----------------------------------------------------------------------------
// File: CGameTimer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Timer.h"

CGameTimer::CGameTimer()
{
	if (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_PerformanceFrequencyPerSec))		// 주파수에 따라 1초당 진행되는 진동 수
	{
		m_bStopped = true;
		::QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastPerformanceCounter);			// 현재 CPU 클럭 수를 알려줌. 시간 간격 측정에 사용
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
	float fTimeElapsed;				// 1frame 경과 시간

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
		::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));		// 초당 프레임 배열 한칸 씩 미루기
		m_fFrameTime[0] = fTimeElapsed;															// 가장 앞 배열에 1frame 경과 시간 대입 
		if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
	}

	m_FramePerSecond++;						// 현재 진행 프레임 (1초 프레임 중)
	m_fFPSTimeElapsed += fTimeElapsed;		// 현재 진행 프레임 경과 시간 (1초 프레임 중)
	//std::cout << "현재 frame: " << m_FramePerSecond << std::endl;

	if (m_fFPSTimeElapsed > 1.0f)			// 초당 프레임 초과 시 초기화
	{
		m_nCurrentFrameRate = m_FramePerSecond;
		m_FramePerSecond = 0;				// 초당 프레임 수 초기화
		m_fFPSTimeElapsed = 0.0f;			// 초당 프레임 경과 시간 초기화
	}

	m_fTimeElapsed = 0.0f;					// 총 frame 경과시간 (Sample Count만큼)
	for (ULONG i = 0; i < m_nSampleCount; ++i) m_fTimeElapsed += m_fFrameTime[i];		// 초당 프레임 경과 시간
	if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;							// 초당 프레임 1frame 평균 경과 시간
	//std::cout << "1 frame 평균 경과 시간 (표본=최근 frame 50개): " << m_fTimeElapsed << std::endl;
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
		_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);		// 정수를 문자열로 변환 (변환 숫자, 변환 결과 출력 버퍼, 버퍼 크기, 기 수 또는 숫자)
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