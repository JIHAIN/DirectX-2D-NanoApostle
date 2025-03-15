#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CTransform.h"

int g_arrKeyValue[(int)KEY::END]
=
{
	'W',
	'S',
	'A',
	'D',
	'Z',
	'X',
	'V',
	'C',
	'E',
	'Y',

	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_SPACE,
	VK_RETURN,

	VK_LSHIFT,

	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,
	VK_NUMPAD0,

	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
};




CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void CKeyMgr::Tick()
{
	// 게임 윈도우가 포커싱중일때만 키 관련 계산이 동작하도록 한다.
	if (GetFocus())
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// 키가 눌려있는지 확인
			if (GetAsyncKeyState(g_arrKeyValue[i]) & 0x8001)
			{
				// 키가 눌려있고, 이전에는 눌려있지 않았다.
				if (false == m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				// 키가 눌려있고, 이전에도 눌려있었다.
				else
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}

				m_vecKey[i].PrevPressed = true;
			}

			// 키가 눌려있지 않다면
			else
			{
				// 이전에는 눌려있었다.
				if (m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::RELEASED;
				}
				else
				{
					m_vecKey[i].State = KEY_STATE::NONE;
				}

				m_vecKey[i].PrevPressed = false;
			}
		}

		m_MousePrevPos = m_MousePos;


		POINT Pos;
		GetCursorPos(&Pos);
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &Pos);
		m_MousePos = Vec2((float)Pos.x, (float)Pos.y);
		m_MouseDir = m_MousePos - m_MousePrevPos;

		Vec2 kResolution = CDevice::GetInst()->GetRenderResolution();
		Vec2 rMousePos = Vec2((float)Pos.x - kResolution.x * 0.5f, kResolution.y * 0.5f - (float)Pos.y);
		// 클라이언트 기준의 좌상단이 0,0인 마우스를 중심 기준으로 바꿔줌(Y축 부호 반전이 있어서 빼는 순서를 반대로)

		// 만든 마우스 상수 버퍼에 바인딩
		static CConstBuffer* mCB = CDevice::GetInst()->GetCB(CB_TYPE::MOUSE);
		g_MouseInfo.MousePos = rMousePos;
		mCB->SetData(&g_MouseInfo);
		mCB->Binding();

		// 마우스의 위치를 카메라 움직임의 보정을 받게 만듬 
		vector<CCamera*> Cam = CRenderMgr::GetInst()->GetVecCamer();

		if (Cam.size() != 0)
		{
		
			for (int i = 0; i < Cam.size(); ++i)
			{
				if (Cam[i] != nullptr)
				{
					CCamera* Cam0 = Cam[i];
					Vec3 CamPos = Cam0->Transform()->GetWorldPos();
					m_MousePosCam = Vec2(m_MousePos.x + CamPos.x, m_MousePos.y + CamPos.y);
					break;
				}
			}
			
		}
	}

	// 게임 윈도우가 포커싱중이 아닐 경우
	else
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].State || KEY_STATE::PRESSED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::RELEASED;
			}

			else if (KEY_STATE::RELEASED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::NONE;
			}

			m_vecKey[i].PrevPressed = false;
		}
	}
}