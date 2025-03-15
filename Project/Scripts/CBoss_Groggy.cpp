#include "pch.h"
#include "CBoss_Groggy.h"
#include "CUI_BossHP.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayer_Ai.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CBoss_Groggy::CBoss_Groggy()
	: CScript((UINT)SCRIPT_TYPE::BOSS_GROGGY)
	, m_Death(false)
{
}

CBoss_Groggy::~CBoss_Groggy()
{
}


void CBoss_Groggy::Begin()
{
	m_Death = false;
	SetAI(true);
}

void CBoss_Groggy::AI_In()
{
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Groggy, 6.f, false);

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* HPObj = CurLevel->FindObjectByName(L"BossHP");
	CUI_BossHP* Hpbar = (CUI_BossHP*)HPObj->GetScript(SCRIPT_TYPE::UI_BOSSHP);
	m_Death = Hpbar->IsDeath();

	if (m_Death == true)
	{
		FlipbookPlayer()->SetTerm(7, 3.f);
	}
}

void CBoss_Groggy::AI_Tick()
{
	if (!m_Death && FlipbookPlayer()->isFinish())
	{
		B_CHANGE(SCRIPT_TYPE::BOSS_IDLE);
		return;
	}
	else if (m_Death && KEY_TAP(KEY::E))
	{
		Vec3 Pos = Transform()->GetRelativePos();
		Pos.y -= 220.f;
		CGameObject* Player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
		Player->Transform()->SetRelativePos(Pos);
		CPlayer_Ai* PlayerScript = (CPlayer_Ai*)Player->GetScript((UINT)SCRIPT_TYPE::PLAYER_AI);
		PlayerScript->ChangeAi(SCRIPT_TYPE::PLAYER_END);
		

		B_CHANGE(SCRIPT_TYPE::BOSS_DEAD);
		return;
	}
}

void CBoss_Groggy::AI_Exit()
{
}

void CBoss_Groggy::SaveComponent(FILE* _File)
{
}

void CBoss_Groggy::LoadComponent(FILE* _File)
{
}

void CBoss_Groggy::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Groggy::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Groggy::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

