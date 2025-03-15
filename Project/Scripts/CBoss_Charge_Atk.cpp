#include "pch.h"
#include "CBoss_Charge_Atk.h"
#include "CBoss_Ai.h"

#include "CBoss_Charge_Atk_Obj.h"

CBoss_Charge_Atk::CBoss_Charge_Atk()
	: CScript((UINT)SCRIPT_TYPE::BOSS_CHARGE_ATK)
	, m_Atk(false)
{
}

CBoss_Charge_Atk::~CBoss_Charge_Atk()
{
}


void CBoss_Charge_Atk::Begin()
{
	SetAI(true);
	m_Atk = false;
}

void CBoss_Charge_Atk::AI_In()
{
	m_Atk = false;
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Charge_Atk, 13.f, false);

}

void CBoss_Charge_Atk::AI_Tick()
{
	if (FlipbookPlayer()->GetCurIdx() == 7 && !m_Atk)
	{
		CreateAtk();
		m_Atk = true;
	}

	if (FlipbookPlayer()->isFinish())
	{
		B_CHANGE(SCRIPT_TYPE::BOSS_IDLE);
		return;
	}
}


void CBoss_Charge_Atk::AI_Exit()
{
}


void CBoss_Charge_Atk::SaveComponent(FILE* _File)
{
}

void CBoss_Charge_Atk::LoadComponent(FILE* _File)
{
}

void CBoss_Charge_Atk::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	
}

void CBoss_Charge_Atk::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Charge_Atk::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Charge_Atk::CreateAtk()
{
	CGameObject* MissileObj = new CGameObject;

	MissileObj->AddComponent(new CMeshRender);
	MissileObj->AddComponent(new CBoss_Charge_Atk_Obj);
	MissileObj->AddComponent(new CLight2D);
	MissileObj->AddComponent(new CCollider2D);
	MissileObj->SetName(L"Boss_Atk");

	Vec3 Pos = Transform()->GetRelativePos();
	Pos.z -= 1.f;

	MissileObj->Transform()->SetRelativePos(Pos);
	MissileObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
	MissileObj->Transform()->SetIndependentScale(true);
	MissileObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	MissileObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossSkillMtrl"));

	// CAssetMgr 의 재질을 복사시킨 재질을 참조
	MissileObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\CircleGradient.png"));

	MissileObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	MissileObj->Light2D()->SetLightColor(Vec3(1.f, 1.f, 0.f));
	MissileObj->Light2D()->SetRadius(30.f);

	MissileObj->Collider2D()->SetIndependentScale(true);
	MissileObj->Collider2D()->SetScale(Vec2(100.f, 100.f));

	CreateObject(MissileObj, 6, false);

	CGameObject* Lightningffect = new CGameObject;
	Lightningffect->AddComponent(new CMeshRender);
	Lightningffect->SetName(L"pAtkT");
	Lightningffect->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Lightningffect->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LightningffectMtrl"));
	Lightningffect->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Thunder.png"));
	Lightningffect->Transform()->SetRelativeScale(Vec3(1.3f, 1.3f, 1.f));
	CreateObject(Lightningffect, 0, false);

	AddChild(MissileObj, Lightningffect);
}


