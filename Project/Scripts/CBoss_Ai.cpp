#include "pch.h"
#include "CBoss_Ai.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CTaskMgr.h>

#include "CPlayer_Ai.h"
#include "CUI_BossHP.h"

bool CBoss_Ai::m_Left = false;

CBoss_Ai::CBoss_Ai()
	: CScript((UINT)SCRIPT_TYPE::BOSS_AI)
	, m_CurScript(nullptr)
	, m_TargetPlayer(nullptr)
	, m_PlayerAi(nullptr)
{
}

CBoss_Ai::~CBoss_Ai()
{
}

void CBoss_Ai::Begin()
{
	m_CurScript = GetOwner()->GetScript((UINT)SCRIPT_TYPE::BOSS_ROAR);

	if (m_CurScript == nullptr)
	{
		return;
	}

	m_CurScript->AI_In();

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_TargetPlayer = CurLevel->FindObjectByName(L"Player");

	m_PlayerAi = (CPlayer_Ai*)m_TargetPlayer->GetScript(SCRIPT_TYPE::PLAYER_AI);
}

void CBoss_Ai::Tick()
{
	if (m_PlayerAi->IsDeathState()) //플레이어가 데스 스크립트 상태인지 확인
		return;

	Vec3 BossScale = Transform()->GetRelativeScale();
	Vec3 PlayerPos = m_TargetPlayer->Transform()->GetRelativePos();
	Vec3 BossPos = GetOwner()->Transform()->GetRelativePos();

	if (m_CurScript != GetOwner()->GetScript(SCRIPT_TYPE::BOSS_DEAD))
	{
		if (PlayerPos.x < BossPos.x) // 플레이어가 나 보다 왼쪽이면 왼쪽 쳐다봄
		{
			if (BossScale.x > 0.f)
			{
				BossScale.x *= -1.f;

				GetOwner()->Transform()->SetRelativeScale(BossScale);
				m_Left = true;
			}
		}
		else // 아니면 오른쪽 봄
		{
			if (BossScale.x < 0.f)
			{
				BossScale.x *= -1.f;

				GetOwner()->Transform()->SetRelativeScale(BossScale);
				m_Left = false;
			}
		}
	}
	else
	{
		if (BossScale.x < 0.f)
		{
			BossScale.x *= -1.f;

			GetOwner()->Transform()->SetRelativeScale(BossScale);
			m_Left = false;
		}
	}

	if (nullptr != m_CurScript)
		m_CurScript->AI_Tick();


}




void CBoss_Ai::SaveComponent(FILE* _File)
{
}

void CBoss_Ai::LoadComponent(FILE* _File)
{
}



void CBoss_Ai::ChangeAi(SCRIPT_TYPE _Type)
{
	CScript* pNextAi = GetOwner()->GetScript(_Type);

	// 기존 상태 나오기 Exit 함수 실행
	if (nullptr != m_CurScript)
	{
		m_CurScript->AI_Exit();
	}

	//새로운 상태로
	m_CurScript = pNextAi;
	assert(m_CurScript);

	// 진입해주기
	m_CurScript->AI_In();
}

void CBoss_Ai::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 4)
	{
		CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CGameObject* HPObj = CurLevel->FindObjectByName(L"BossHP");
		CUI_BossHP* Hpbar = (CUI_BossHP*)HPObj->GetScript(SCRIPT_TYPE::UI_BOSSHP);
		Hpbar->HitHP(5.f);

		if (_OtherObject->GetName() == L"reflection")
		{
			ChangeAi(SCRIPT_TYPE::BOSS_GROGGY);
		}
	}
	
}

void CBoss_Ai::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Ai::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

