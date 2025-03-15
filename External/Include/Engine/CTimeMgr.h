#pragma once


class CTimeMgr
	: public singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_Frequency;	// 1�ʴ� ī���� ���� ��
	LARGE_INTEGER	m_CurrentCount;	// ���� ī����
	LARGE_INTEGER	m_PrevCount;	// ���� ������ ī����
		
	float			m_fDT;			// DeltaTime : 1�����ӿ� �ɸ� �ð�
	float			m_Time;			// ���� �ð�
	float			m_fEngineDT;	
	float			m_fEngineTime;

	double			m_Second;	// 1�� üũ �뵵 �����ð�

	UINT			m_FPS;

	bool			m_IsStop;
	bool			m_Slow;
	wchar_t			m_TimeInfo[255];
public:
	void Init();
	void Tick();
	void Render();
	bool IsSlow() { return m_Slow; }
	void SlowTime() { m_Slow = !m_Slow;}

	void SetStopMode(bool _IsStop) 
	{ 
		m_IsStop = _IsStop; 
		if(m_IsStop)
			m_fDT = 0.f;
	}

public:	
	float GetDeltaTime() { return m_fDT; }
	float GetEngineDeltaTime() { return m_fEngineDT; }

};