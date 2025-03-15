#include "pch.h"
#include "CTurretScript.h"
#include "CTurretMissileScript.h"

#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayer_Ai.h"



CTurretScript::CTurretScript()
	: CScript((UINT)SCRIPT_TYPE::TURRETSCRIPT)
	, AccTime(0.f)
	, m_TargetPlayer(nullptr)
	, m_Groggy(false)
	, m_First(true)
{

}

CTurretScript::~CTurretScript()
{

}

void CTurretScript::Begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_TargetPlayer = CurLevel->FindObjectByName(L"Player");
	m_Groggy = false;
	m_First = true;

}

void CTurretScript::Tick()
{
	AccTime += DT;

	if (AccTime > 1.f)
	{
		if (pParticle != nullptr)
		{
			DestroyObject(pParticle);
			pParticle = nullptr;
		}
	}

	if (m_Groggy && KEY_TAP(KEY::E) && m_First)
	{
		m_First = false;

		CPlayer_Ai* PlayerAi = (CPlayer_Ai*)m_TargetPlayer->GetScript(SCRIPT_TYPE::PLAYER_AI);
		PlayerAi->Set_Groggy_Monster(GetOwner());
	}
	else if (m_Groggy)
		return;


	Vec3 PlayerPos = m_TargetPlayer->Transform()->GetWorldPos();
	float Diff = PlayerPos.x - Transform()->GetWorldPos().x;
	// 플레이어랑 나의 위치 차이가 0보다 작고 -100보다 클때 플레이어가 왼쪽에서 나랑 x값 차이가 100이하일때 생성하게 함

	if (Diff <= 0.f && Diff >= -700.f && !m_Groggy)
	{
		if (AccTime >= 2.f)
		{
			pParticle = new CGameObject;
			pParticle->SetName(L"Turret_Particle");
			pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare8_1024.png")));
			pParticle->Transform()->SetRelativePos(0.f, 0.f, 1.f);
			CreateObject(pParticle, 2, false);
			AddChild(GetOwner(), pParticle);

			CGameObject* pMissileObject = new CGameObject;
			pMissileObject->AddComponent(new CMeshRender);
			pMissileObject->AddComponent(new CTurretMissileScript);
			pMissileObject->AddComponent(new CLight2D);
			pMissileObject->AddComponent(new CCollider2D);
			pMissileObject->SetName(L"Missile");
			// 스크립트 이름으로 찾아서 가져오는 함수 만듬.
			CTurretMissileScript* pMissilScript = (CTurretMissileScript*)pMissileObject->GetScript((UINT)SCRIPT_TYPE::TURRETMISSILESCRIPT);
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
	

}

void CTurretScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"reflection")
	{
		m_Groggy = true;
	}
}

void CTurretScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CTurretScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


tParticleModule CTurretScript::ParticleSetting()
{
	// Flare8_1024.png
	tParticleModule m_Module = {};

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Module.MinLife = 0.f;
	m_Module.MaxLife = 0.8f;
	m_Module.vSpawnMinScale = Vec3(30.f, 30.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(30.f, 30.f, 1.f);

	m_Module.SpawnShape = 0;
	m_Module.SpawnShapeScale.x = 0;
	m_Module.SpawnShapeScale.y = 0;

	m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
	m_Module.BlockSpawnShapeScale.x = 0;
	m_Module.BlockSpawnShapeScale.y = 0;

	m_Module.SpaceType = 0; // Local Space 

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 3;
	m_Module.AddVelocityFixedDir = Vec3(0.1f, 0.1f, 0); // 이걸 내 방향 기준으로
	m_Module.AddMinSpeed = 0.001;
	m_Module.AddMaxSpeed = 0.001;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec4(0.4f, 0.4f, 0.4f, 0.01f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.7f;
	m_Module.VelocityAlignment = false;

	// SPAWN BURST
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstCount = 1;
	m_Module.SpawnBurstRepeat = false;
	m_Module.SpawnBurstRepeatTime = 60.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 10.f;
	m_Module.EndScale = 1.f;

	// Drag Module (감속)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 1.f;
	m_Module.LimitSpeed = 2.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm = 0.3f;
	m_Module.NoiseForceScale = 600.f;

	return m_Module;
}