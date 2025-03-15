#include "pch.h"
#include "CBoss_Charge_Atk_Obj.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CTaskMgr.h>

#include "CShield_Script_Obj.h"


CBoss_Charge_Atk_Obj::CBoss_Charge_Atk_Obj()
	: CScript((UINT)SCRIPT_TYPE::BOSS_CHARGE_ATK_OBJ)
{
}

CBoss_Charge_Atk_Obj::~CBoss_Charge_Atk_Obj()
{
}


void CBoss_Charge_Atk_Obj::Begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_TargetPlayer = CurLevel->FindObjectByName(L"Player");

	if (m_TargetPlayer == nullptr)
		return;

	Vec3 m_TargetPos = m_TargetPlayer->Transform()->GetRelativePos();
	m_Velocity = m_TargetPos - Transform()->GetRelativePos();
	m_Velocity.Normalize();
	
	m_Originalsize = Transform()->GetRelativeScale();
	m_DeleteTime = 0.f;
	m_ScaleTime = 0.f;
}

void CBoss_Charge_Atk_Obj::Tick()
{
	m_DeleteTime += DT;
	m_ScaleTime += DT;

	if (m_DeleteTime <= 0.3f) // 0.5 초동안은 유도탄역활을함
	{
		Vec3 m_TargetPos = m_TargetPlayer->Transform()->GetRelativePos();
		m_Velocity = m_TargetPos - Transform()->GetRelativePos();
		m_Velocity.Normalize();
	}
	else if (m_DeleteTime >= 1.4f)
	{
		DestroyObject(GetOwner());

		m_DeleteTime = 0;
	}

	Vec3 vScale = Transform()->GetRelativeScale();
	if (m_ScaleTime >= 0.15f && !m_smaller)
	{
		vScale *= 0.9f;
		m_ScaleTime = 0.f;
		m_smaller = true;
	}
	else if (m_ScaleTime >= 0.15f && m_smaller)
	{
		vScale = m_Originalsize;
		m_ScaleTime = 0.f;
		m_smaller = false;
	}


	Vec3 vWorldPos = Transform()->GetRelativePos();
	vWorldPos += m_Velocity * 1000 * DT;

	Transform()->SetRelativePos(vWorldPos);
	Transform()->SetRelativeScale(vScale);
}

void CBoss_Charge_Atk_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_TargetPlayer)
	{
		if (_OtherObject->GetName() == m_TargetPlayer->GetName())
		{
			DestroyObject(GetOwner());

			// Sound 로딩하기
			Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\TraceBombExplosion.wav", L"Sound\\TraceBombExplosion.wav");
			pSound->Play(1, 0.5f, true);
		}
	}

	if (_OtherObject->GetName() == L"Shield")
	{
		CShield_Script_Obj* OtherShield = (CShield_Script_Obj*)_OtherObject->GetScript(SCRIPT_TYPE::SHIELD_SCRIPT_OBJ);

		if (OtherShield->IsParry())
		{
			CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* Boss = CurLevel->FindObjectByName(L"Boss");

			Vec3 m_TargetPos = Boss->Transform()->GetRelativePos();
			m_Velocity = m_TargetPos - Transform()->GetRelativePos();
			m_Velocity.Normalize();
			GetOwner()->SetName(L"reflection");
			GetOwner()->SetLayerIdx(4);
		}

	}
	else if (GetOwner()->GetName() == L"reflection" && _OtherObject->GetName() == L"Boss") // 반격상태에서 보스에게 닿을시
	{
		DestroyObject(GetOwner());

		// Sound 로딩하기
		Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\TraceBombExplosion.wav", L"Sound\\TraceBombExplosion.wav");
		pSound->Play(1, 0.5f, true);
	}

}

void CBoss_Charge_Atk_Obj::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Charge_Atk_Obj::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
