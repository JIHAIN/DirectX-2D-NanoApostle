#include "pch.h"
#include "CTurretMissileScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CTaskMgr.h>

#include "CShield_Script_Obj.h"


CTurretMissileScript::CTurretMissileScript()
	: CScript((UINT)SCRIPT_TYPE::TURRETMISSILESCRIPT)
	, m_Velocity(Vec3(0.f, 0.f, 0.f))
	, m_TargetPlayer(nullptr)
	, m_DeleteTime(0.f)
	, m_ScaleTime(0.f)
	, m_smaller(false)
	, m_Speed(500.f)
	, m_Originalsize(0.f, 0.f, 0.f)
	, m_TurretPos(0.f, 0.f, 0.f)
{
}

CTurretMissileScript::~CTurretMissileScript()
{
}

void CTurretMissileScript::Begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_TargetPlayer = CurLevel->FindObjectByName(L"Player");

	if (m_TargetPlayer == nullptr)
		return;

	Vec3 m_TargetPos = m_TargetPlayer->Transform()->GetRelativePos();

	m_Velocity = m_TargetPos - m_TurretPos;

	m_Velocity.Normalize();
	m_Speed = 500.f;

	// CAssetMgr 의 재질을 복사시킨 재질을 참조
	//MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Missile", L"Texture\\CircleGradient.png"));


	m_Originalsize = Transform()->GetRelativeScale();
}

void CTurretMissileScript::Tick()
{
	m_DeleteTime += DT;
	m_ScaleTime += DT;

	if (m_DeleteTime >= 2.f)
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
	vWorldPos += m_Velocity * m_Speed * DT;

	Transform()->SetRelativePos(vWorldPos);
	Transform()->SetRelativeScale(vScale);
}

void CTurretMissileScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Shield")
	{
		CShield_Script_Obj* OtherShield = (CShield_Script_Obj*)_OtherObject->GetScript(SCRIPT_TYPE::SHIELD_SCRIPT_OBJ);

		if (OtherShield->IsParry())
		{
			m_Velocity *= -1.f;
			m_Speed = 1000.f;
			GetOwner()->SetName(L"reflection");
			GetOwner()->SetLayerIdx(4);
		}

	}
	else if (_OtherObject->GetName() == m_TargetPlayer->GetName() || GetOwner()->GetParent()->GetName() == _OtherObject->GetName())
	{
		DestroyObject(GetOwner());
	}
	

}

void CTurretMissileScript::SaveComponent(FILE* _File)
{
}

void CTurretMissileScript::LoadComponent(FILE* _File)
{
}
