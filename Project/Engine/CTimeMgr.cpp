#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CFontMgr.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_CurrentCount{}
	, m_PrevCount{}	
	, m_fDT(0.f)
	, m_Time(0.f)
	, m_fEngineDT(0.f)
	, m_fEngineTime(0.f)
	, m_FPS(0)
	, m_Second(0.)
	, m_IsStop(true)
	, m_Slow(false)
	, m_TimeInfo{}
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_CurrentCount);
	m_fEngineDT = (float)(m_CurrentCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart;

	// 프레임 제한 (100fps)
	const float targetFrameTime = 1.0f /100.0f;

	if (m_fEngineDT < targetFrameTime)
	{
		// Sleep으로 대략적인 시간 맞추기
		float sleepTime = (targetFrameTime - m_fEngineDT) * 1000.0f;
		if (sleepTime > 1.0f)  // 1ms 이상 남았을 때만 Sleep
		{
			Sleep((DWORD)(sleepTime - 1.0f));  // 오차 감안해서 1ms 빼고 sleep
		}

		// 나머지는 스핀락으로 정확히 맞추기
		LARGE_INTEGER targetCount;
		targetCount.QuadPart = m_PrevCount.QuadPart + (LONGLONG)(targetFrameTime * m_Frequency.QuadPart);

		while (m_CurrentCount.QuadPart < targetCount.QuadPart)
		{
			QueryPerformanceCounter(&m_CurrentCount);
		}

		// 실제 경과 시간 재계산
		m_fEngineDT = (float)(m_CurrentCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart;
	}

	// 아래로는 원래코드
	// QueryPerformanceCounter(&m_CurrentCount);
	//m_fEngineDT = (float)(m_CurrentCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart; 
	
	// DT 보정, 60 분의 1 보다 크지 못하게 보정
	if ((1.f / 60.f) < m_fEngineDT)
		m_fEngineDT = (1.f / 60.f);
		
	// 시간 누적
	m_fEngineTime += m_fEngineDT;

	// 시간 누적
	m_Second += m_fEngineDT;

	// 함수 호출횟수
	++m_FPS;

	if (1.f < m_Second)
	{
		m_Second -= 1.;

		swprintf_s(m_TimeInfo, L"DeltaTime : %f, FPS : %d ", m_fEngineDT, m_FPS);

		m_FPS = 0;
	}

	m_PrevCount = m_CurrentCount;

	if (!m_IsStop)
	{
		if (m_Slow)
		{
			m_fDT = m_fEngineDT/4;
			m_Time += m_fDT;
		}
		else
		{
			m_fDT = m_fEngineDT;
			m_Time += m_fDT;
		}
		
	}	

	// GlobalData 갱신
	g_Data.DeltaTime = m_fDT;
	g_Data.Time = m_Time;

	g_Data.DT_Engine = m_fEngineDT;
	g_Data.Time_Engine = m_fEngineTime;
}
void CTimeMgr::Render()
{
	CFontMgr::GetInst()->DrawFont(m_TimeInfo, 10, 20, 16, FONT_RGBA(100, 100, 100, 255));
}