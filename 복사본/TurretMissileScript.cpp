#include "pch.h"
#include "TurretMissileScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CTaskMgr.h>

TurretMissileScript::TurretMissileScript()
	: CScript((UINT)SCRIPT_TYPE::TURRET_MISSILE_SCRIPT)
	, m_Velocity(Vec3(0.f, 0.f, 0.f))
	, m_TargetPlayer(nullptr)
	, m_DeleteTime(0.f)
	, m_ScaleTime(0.f)
	, m_smaller(false)
	, m_Originalsize(0.f, 0.f, 0.f)
	, m_TurretPos(0.f, 0.f, 0.f)
{
}

TurretMissileScript::~TurretMissileScript()
{
}

void TurretMissileScript::Begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_TargetPlayer = CurLevel->FindObjectByName(L"Player");

	Vec3 m_TargetPos = m_TargetPlayer->Transform()->GetRelativePos();

	m_Velocity = m_TargetPos - m_TurretPos;

	m_Velocity.Normalize();

	// CAssetMgr 의 재질을 복사시킨 재질을 참조
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	MeshRender()->SetMaterial(pMtrl);
	MeshRender()->GetDynamicMaterial();

	Ptr<CTexture> pMissileTex = CAssetMgr::GetInst()->Load<CTexture>(L"Missile", L"Texture\\CircleGradient.png");
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, pMissileTex);

	m_Originalsize = Transform()->GetRelativeScale();
}

void TurretMissileScript::Tick()
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
		vScale *= 0.6f;
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
	vWorldPos += m_Velocity * 500 * DT;

	Transform()->SetRelativePos(vWorldPos);
	Transform()->SetRelativeScale(vScale);
}

void TurretMissileScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_TargetPlayer)
	{
		if (_OtherObject->GetName() == m_TargetPlayer->GetName())
		{

			m_Velocity *= -1;
		}
	}
}
