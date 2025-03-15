#include "pch.h"
#include "TurretScript.h"
#include "TurretMissileScript.h"

#include <Engine/CTaskMgr.h>

TurretScript::TurretScript()
	: CScript((UINT)SCRIPT_TYPE::TURRET_SCRIPT)
	, AccTime(0.f)
{

}

TurretScript::~TurretScript()
{

}

void TurretScript::Begin()
{

}

void TurretScript::Tick()
{
	AccTime += DT;

	if (AccTime >= 2.f)
	{
		CGameObject* pMissileObject = new CGameObject;

		pMissileObject->AddComponent(new CMeshRender);
		pMissileObject->AddComponent(new TurretMissileScript);
		pMissileObject->AddComponent(new CLight2D);
		pMissileObject->AddComponent(new CCollider2D);
		pMissileObject->SetName(L"Missile");
		// 스크립트 이름으로 찾아서 가져오는 함수 만듬.
		TurretMissileScript* pMissilScript = (TurretMissileScript*)pMissileObject->GetScript(L"TurretMissileScript");
		pMissilScript->SetTurretPos(Transform()->GetRelativePos());

		pMissileObject->Transform()->SetRelativePos(0.f, 0.f, 1.f);
		pMissileObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
		pMissileObject->Transform()->SetIndependentScale(true);
		pMissileObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));

		pMissileObject->Light2D()->SetLightType(LIGHT_TYPE::POINT);
		pMissileObject->Light2D()->SetLightColor(Vec3(0.5f, 0.5f, 0.5f));
		pMissileObject->Light2D()->SetRadius(40.f);

		pMissileObject->Collider2D()->SetIndependentScale(true);
		pMissileObject->Collider2D()->SetScale(Vec2(60.f, 60.f));
		
		CreateObject(pMissileObject, 6, false);

		AddChild(GetOwner(), pMissileObject);

		AccTime = 0.f;
	}

}

void TurretScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void TurretScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void TurretScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

// 터렛 구성
//CGameObject* pTurret = new CGameObject;
//pTurret->SetName(L"Turret1");
//pTurret->AddComponent(new CMeshRender);
//pTurret->AddComponent(new TurretScript);
//pTurret->AddComponent(new CCollider2D);
//
//pTurret->Transform()->SetRelativePos(200.f, 30.f, 10.f);
//pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
//
//pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
//pTurret->MeshRen
