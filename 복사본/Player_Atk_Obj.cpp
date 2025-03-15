#include "pch.h"
#include "Player_Atk_Obj.h"

#include <Engine/CSprite.h>
#include <Engine/CFlipbook.h>

Player_Atk_Obj::Player_Atk_Obj()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ATK_SCRIPT)
	, m_DeleteTime(0.f)
	, m_OriginRot(0.f, 0.f, 0.f)
	, m_SpriteRotfrist(true)
	, m_Name(L" ")
{
	
}

Player_Atk_Obj::~Player_Atk_Obj()
{
}

void Player_Atk_Obj::Begin()
{
	m_OriginRot = GetOwner()->Transform()->GetRelativeRotation();
	m_Name = GetOwner()->GetName();
}

void Player_Atk_Obj::Tick()
{
	m_DeleteTime += DT;

	Vec3 Rota = GetOwner()->Transform()->GetRelativeRotation();

	Ptr<CSprite> CurSpr = GetOwner()->FlipbookPlayer()->GetCurrentSprite();
	
	if (CurSpr != nullptr && m_SpriteRotfrist)
	{
		wstring Key = CurSpr.Get()->GetKey();

		if (Key == L"AESlash4")
		{

			if (m_Name == L"pAtk1")
			{
				Rota.y *= -1.f;
				Rota.z -= XM_PI / 3.33f;
			}
			else if (m_Name == L"pAtk2")
			{
				Rota.y *= -1.f;
				Rota.z -= XM_PI / 3.33f;
			}
			else if (m_Name == L"pAtk3_1")
			{
				// 3��° ������ ùŸ Ʋ����� ����
				Rota.x = 0.f;
				Rota.y = XM_PI / 4.f;
				Rota.z *= -1.f;
				Rota.z *= 2.f;

				// 3_1 ùŸ�� ������ Ʋ������ 3_2 ��ü �����϶�� ��������Ʈ�� �˷��ֱ�
				if (m_DlgtInst && m_DlgtMemFunc)
				{
					(m_DlgtInst->*m_DlgtMemFunc)();
				}
			}
			else if (m_Name == L"pAtk3_2")
			{
				Rota.z -= XM_PI;
			}
			/*	Rota.z += XM_PI;*/

			m_SpriteRotfrist = false;
		}
		
	}

	GetOwner()->Transform()->SetRelativeRotation(Rota);

	if (GetOwner()->FlipbookPlayer()->isFinish())
	{
		DestroyObject(GetOwner());

		m_DeleteTime = 0;
	}

	//if (m_DeleteTime >= 0.4f)
	//{
	//	DestroyObject(GetOwner());

	//	m_DeleteTime = 0;
	//}
}

void Player_Atk_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

// �밢�� �Ʒ� 2Ÿ ���� ������ ���� x 200, y 400, 1
// �����̼� 0 ,200, 30

// ���� 1Ÿ ������ 400, 190, 0, 0, -200
// ���� 2Ÿ ������ 200 400 , 0 -200 - 60