#include "pch.h"
#include "CPlayer_Atk_Obj.h"

#include <Engine/CSprite.h>
#include <Engine/CFlipbook.h>


CPlayer_Atk_Obj::CPlayer_Atk_Obj()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ATK_OBJ)
	, m_SpriteRotfrist(true)
	, m_Name(L" ")
	, m_DlgtInst(nullptr)
	, m_DlgtMemFunc(nullptr)
	, pParticle(nullptr)
	, m_EffectDodge(nullptr)
	, m_EffectBlood(nullptr)
{
}


CPlayer_Atk_Obj::~CPlayer_Atk_Obj()
{
	if (pParticle != nullptr)
	{
		DestroyObject(pParticle);
		pParticle = nullptr;
	}
	if (m_EffectDodge != nullptr)
	{
		DestroyObject(m_EffectDodge);
		m_EffectDodge = nullptr;
	}
	if (m_EffectBlood != nullptr)
	{
		DestroyObject(m_EffectBlood);
		m_EffectBlood = nullptr;
	}
	
}

tParticleModule CPlayer_Atk_Obj::ParticleSetting()
{
	tParticleModule m_Module = {};

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(0.7f, 0.f, 0.f, 1.f);
	m_Module.MinLife = 0.4f;
	m_Module.MaxLife = 0.4f;
	m_Module.vSpawnMinScale = Vec3(100.f, 100.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(100.f, 100.f, 1.f);

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
	m_Module.EndColor = Vec4(0.65f, 0.f, 0.f, 0.01f);
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
	m_Module.StartScale = 3.f;
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

tParticleModule CPlayer_Atk_Obj::ParticleSetting1(Vec3 _Dir)
{
	Vec2 tSpawnShapeScale = Vec2(0.f, 0.f); // Box

	// 파티클 기능(모듈) 정보 세팅
			// Spawn Module
	tParticleModule m_Module;

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(0.7f, 0.f, 0.f, 0.8f);
	m_Module.MinLife = 0.f;
	m_Module.MaxLife = 0.7f;
	m_Module.vSpawnMinScale = Vec3(3.f, 3.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(8.f, 8.f, 1.f);

	m_Module.SpawnShape = 0;
	m_Module.SpawnShapeScale.x = 100.f;
	m_Module.SpawnShapeScale.y = 100.f;

	m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
	m_Module.BlockSpawnShapeScale.x = 50.f;
	m_Module.BlockSpawnShapeScale.y = 50.f;

	m_Module.SpaceType = 0; // Local Space 

	// SPAWN BURST
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstCount = 150;
	m_Module.SpawnBurstRepeat = false;
	m_Module.SpawnBurstRepeatTime = 60.f;

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 3;
	m_Module.AddVelocityFixedDir = _Dir; // 이걸 내 방향 기준으로
	m_Module.AddMinSpeed = 200.f;
	m_Module.AddMaxSpeed = 600.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec4(0.65f, 0.f, 0.f, 0.01f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.1f;
	m_Module.VelocityAlignment = false;

	

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 4.5f;
	m_Module.EndScale = 1.f;

	// Drag Module (감속)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 5.f;
	m_Module.LimitSpeed = 0.01f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
	m_Module.NoiseForceTerm = 0.1f;
	m_Module.NoiseForceScale = 600.f;

	return m_Module;
}

void CPlayer_Atk_Obj::Begin()
{
	m_Name = GetOwner()->GetName();
	m_EffectDodge = nullptr;
	pParticle = nullptr;
	m_EffectBlood = nullptr;


}

void CPlayer_Atk_Obj::Tick()
{
	Vec3 Rota = GetOwner()->Transform()->GetRelativeRotation();

	Ptr<CSprite> CurSpr = GetOwner()->FlipbookPlayer()->GetCurrentSprite();
	

	if (m_EffectDodge != nullptr)
	{
		Vec3 Scale = m_EffectDodge->Transform()->GetRelativeScale();
		if (Scale.x > 0.f)
		{
			Scale.x -= DT * 1000;
			Scale.y -= DT * 1000;
		}
		m_EffectDodge->Transform()->SetRelativeScale(Scale);
	}

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
				// 3번째 공격의 첫타 틀어야할 방향
				Rota.x = 0.f;
				Rota.y = XM_PI / 4.f;
				Rota.z *= -1.f;
				Rota.z *= 2.f;

				// 3_1 첫타의 방향이 틀어질때 3_2 객체 생성하라고 델리게이트로 알려주기
				if (m_DlgtInst && m_DlgtMemFunc)
				{
					(m_DlgtInst->*m_DlgtMemFunc)();
				}

			}
			else if (m_Name == L"pAtk3_2")
			{
				Rota.z -= XM_PI;
			}

			m_SpriteRotfrist = false;
		}
		
	}

	GetOwner()->Transform()->SetRelativeRotation(Rota);

	if (GetOwner()->FlipbookPlayer()->isFinish())
	{
		DestroyObject(GetOwner());
	}
}

void CPlayer_Atk_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Boss" && m_EffectDodge == nullptr)
	{
		Vec3 myPos = _Collider->Transform()->GetWorldPos();
		Vec3 otherPos = _OtherCollider->Transform()->GetWorldPos();

		// 콜라이더의 AABB(Axis-Aligned Bounding Box) 정보
		Vec2 myScale = _Collider->GetScale();
		Vec2 otherScale = _OtherCollider->GetScale();

		// 각 콜라이더의 경계 계산
		float myLeft = myPos.x - myScale.x * 0.5f;
		float myRight = myPos.x + myScale.x * 0.5f;
		float myTop = myPos.y + myScale.y * 0.5f;
		float myBottom = myPos.y - myScale.y * 0.5f;

		float otherLeft = otherPos.x - otherScale.x * 0.5f;
		float otherRight = otherPos.x + otherScale.x * 0.5f;
		float otherTop = otherPos.y + otherScale.y * 0.5f;
		float otherBottom = otherPos.y - otherScale.y * 0.5f;

		// 겹치는 영역의 중심점 계산
		Vec3 collisionPoint;
		collisionPoint.x = (max(myLeft, otherLeft) + min(myRight, otherRight)) * 0.5f;
		collisionPoint.y = (max(myBottom, otherBottom) + min(myTop, otherTop)) * 0.5f;
		collisionPoint.z = myPos.z;  // z축은 공격 오브젝트의 z값 사용

		pParticle = new CGameObject;
		pParticle->SetName(L"ATK_Particle");
		pParticle->AddComponent(new CParticleSystem(ParticleSetting(),CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Impact5_FullyWhite.png")));
		pParticle->Transform()->SetIndependentScale(true);
		pParticle->Transform()->SetRelativePos(collisionPoint);
		CreateObject(pParticle, 2, false);

		// 공격 방향 벡터 계산
		Vec3 attackDir = (otherPos - myPos).Normalize();

		// 각도 계산 (기본 각도에 공격 방향 반영)
		float baseAngle = -45.f * (XM_PI / 180.f); // 기본 -45도
		float directionAngle = atan2(attackDir.y, attackDir.x);
		float finalAngle = baseAngle + directionAngle;

		// 거리에 따른 스케일 조절
		float baseScale = 340.f;

		// 이펙트 생성 및 설정
		m_EffectDodge = new CGameObject;
		m_EffectDodge->SetName(L"ATK_effect");
		m_EffectDodge->AddComponent(new CMeshRender);
		m_EffectDodge->MeshRender()->SetMesh(L"RectMesh");
		m_EffectDodge->MeshRender()->SetMaterial(L"EffectMtrl");
		m_EffectDodge->Transform()->SetIndependentScale(true);
		m_EffectDodge->Transform()->SetRelativePos(collisionPoint);
		m_EffectDodge->Transform()->SetRelativeScale(Vec3(baseScale, baseScale, 1.f));
		m_EffectDodge->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, finalAngle));
		m_EffectDodge->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\DodgeLine_0.png"));
		m_EffectDodge->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(2.f, 0.8f, 0.f, 0.9f));
		CreateObject(m_EffectDodge, 2, false);


		// 이펙트 생성 및 설정
		m_EffectBlood = new CGameObject;
		m_EffectBlood->SetName(L"Blood_effect");
		m_EffectBlood->AddComponent(new CParticleSystem(ParticleSetting1(attackDir), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\TX_HitFlash_0.png")));
		m_EffectBlood->Transform()->SetIndependentScale(true);
		m_EffectBlood->Transform()->SetRelativePos(myPos);
		CreateObject(m_EffectBlood, 2, false);
	}
	
}

// 대각선 아래 2타 기준 스케일 값은 x 200, y 400, 1
// 로테이션 0 ,200, 30

// 왼쪽 1타 스케일 400, 190, 0, 0, -200
// 왼쪽 2타 스케일 200 400 , 0 -200 - 60

void CPlayer_Atk_Obj::SaveComponent(FILE* _File)
{
	0;
}

void CPlayer_Atk_Obj::LoadComponent(FILE* _File)
{
	m_SpriteRotfrist = true;
	m_DlgtInst = nullptr;
	m_DlgtMemFunc = nullptr;
}
