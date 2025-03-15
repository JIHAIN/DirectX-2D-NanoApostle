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
	// ���� �����찡 ��Ŀ�����϶��� Ű ���� ����� �����ϵ��� �Ѵ�.
	if (GetFocus())
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// Ű�� �����ִ��� Ȯ��
			if (GetAsyncKeyState(g_arrKeyValue[i]) & 0x8001)
			{
				// Ű�� �����ְ�, �������� �������� �ʾҴ�.
				if (false == m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				// Ű�� �����ְ�, �������� �����־���.
				else
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}

				m_vecKey[i].PrevPressed = true;
			}

			// Ű�� �������� �ʴٸ�
			else
			{
				// �������� �����־���.
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
		// Ŭ���̾�Ʈ ������ �»���� 0,0�� ���콺�� �߽� �������� �ٲ���(Y�� ��ȣ ������ �־ ���� ������ �ݴ��)

		// ���� ���콺 ��� ���ۿ� ���ε�
		static CConstBuffer* mCB = CDevice::GetInst()->GetCB(CB_TYPE::MOUSE);
		g_MouseInfo.MousePos = rMousePos;
		mCB->SetData(&g_MouseInfo);
		mCB->Binding();

		// ���콺�� ��ġ�� ī�޶� �������� ������ �ް� ���� 
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

	// ���� �����찡 ��Ŀ������ �ƴ� ���
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