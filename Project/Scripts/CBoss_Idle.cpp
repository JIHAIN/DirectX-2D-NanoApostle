#include "pch.h"
#include "CBoss_Idle.h"
#include "CBoss_Ai.h"

CBoss_Idle::CBoss_Idle()
	: CScript((UINT)SCRIPT_TYPE::BOSS_IDLE)
	, m_Time(0.f)
	, m_first(true)
{
	SetAI(true);

}

CBoss_Idle::~CBoss_Idle()
{
}

void CBoss_Idle::Begin()
{
	SetAI(true);
	m_Time = 0.f;
	m_first = true;
}


void CBoss_Idle::AI_In()
{
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Idle, 11.f, true);
	m_Time = 0.f;
}

void CBoss_Idle::AI_Tick()
{
	m_Time += DT;

	if (m_Time > 1.f)
	{
		//B_CHANGE(SCRIPT_TYPE::BOSS_NOR_ATK1);
		//B_CHANGE(SCRIPT_TYPE::BOSS_BOTTOM_ATK);
		//B_CHANGE(SCRIPT_TYPE::BOSS_CHARGE_ATK);
		//B_CHANGE(SCRIPT_TYPE::BOSS_DEAD);
		//B_CHANGE(SCRIPT_TYPE::BOSS_ROAR);

		// 0 또는 1의 랜덤값 생성
		int randomValue = rand() % 2;

		if (randomValue == 0)
		{
			B_CHANGE(SCRIPT_TYPE::BOSS_JUMP_ATK);
		}
		else
		{
			B_CHANGE(SCRIPT_TYPE::BOSS_DASH);
		}

	}
}

void CBoss_Idle::AI_Exit()
{
}





void CBoss_Idle::SaveComponent(FILE* _File)
{
}

void CBoss_Idle::LoadComponent(FILE* _File)
{
}

void CBoss_Idle::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Idle::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Idle::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
