#include "pch.h"
#include "CShield_Script_Obj.h"

CShield_Script_Obj::CShield_Script_Obj()
	: CScript((UINT)SCRIPT_TYPE::SHIELD_SCRIPT_OBJ)
	, m_Parrying(true)
	, m_ParryingTime(0.5f)
	, pParticle(nullptr)
	, pParticle1(nullptr)
	, pParticle2(nullptr)

{
}

CShield_Script_Obj::~CShield_Script_Obj()
{
	if (pParticle != nullptr)
	{
		DestroyObject(pParticle);
		pParticle = nullptr;
		DestroyObject(pParticle1);
		pParticle1 = nullptr;	
		DestroyObject(pParticle2);
		pParticle2 = nullptr;
	}
}


void CShield_Script_Obj::Begin()
{
	pParticle = nullptr;
	pParticle1 = nullptr;
	pParticle2 = nullptr; 

	m_Parrying = true;
	m_ParryingTime = 0.5f;
}

void CShield_Script_Obj::Tick()
{
	m_ParryingTime -= DT;

	if (m_ParryingTime <= 0.f)
		m_Parrying = false;
}

void CShield_Script_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 6)
	{
		if (pParticle == nullptr && m_Parrying)
		{
			// Sound 로딩하기
			Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\ShieldImpact4.wav", L"Sound\\ShieldImpact4.wav");
			pSound->Play(1, 0.7f, true);

			pParticle = new CGameObject;
			pParticle->SetName(L"Parry_Particle");
			pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Paring.png")));
			pParticle->Transform()->SetRelativePos(Transform()->GetRelativePos());
			CreateObject(pParticle, 2, false);

			pParticle1 = new CGameObject;
			pParticle1->SetName(L"Parry_Particle");
			pParticle1->AddComponent(new CParticleSystem(ParticleSetting1(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare3.png")));
			pParticle1->Transform()->SetRelativePos(Transform()->GetRelativePos());
			CreateObject(pParticle1, 2, false);

			pParticle2 = new CGameObject;
			pParticle2->SetName(L"Parry_Particle");
			pParticle2->AddComponent(new CParticleSystem(ParticleSetting2(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Shield_big2.png")));
			pParticle2->Transform()->SetRelativePos(Transform()->GetRelativePos());
			CreateObject(pParticle2, 2, false);
		}
	}

}

void CShield_Script_Obj::SaveComponent(FILE* _File)
{
}

void CShield_Script_Obj::LoadComponent(FILE* _File)
{
}

tParticleModule CShield_Script_Obj::ParticleSetting()
{
	// Flare8_1024.png
	tParticleModule m_Module = {};

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Module.MinLife = 0.6f;
	m_Module.MaxLife = 0.6f;
	m_Module.vSpawnMinScale = Vec3(70.f, 70.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(70.f, 70.f, 1.f);

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
	m_Module.EndColor = Vec4(1.f, 1.f, 1.f, 0.01f);
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
	m_Module.StartScale = 2.f;
	m_Module.EndScale = 2.5f;

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

tParticleModule CShield_Script_Obj::ParticleSetting1()
{
	tParticleModule m_Module = {};

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.5f);
	m_Module.MinLife = 0.6f;
	m_Module.MaxLife = 0.6f;
	m_Module.vSpawnMinScale = Vec3(160.f, 160.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(160.f, 160.f, 1.f);

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
	m_Module.EndColor = Vec4(1.f, 1.f, 1.f, 0.01f);
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

tParticleModule CShield_Script_Obj::ParticleSetting2()
{
	tParticleModule m_Module = {};

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(0.6f, 0.6f, 0.6f, 0.3f);
	m_Module.MinLife = 0.6f;
	m_Module.MaxLife = 0.6f;
	m_Module.vSpawnMinScale = Vec3(110.f, 110.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(110.f, 110.f, 1.f);

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
	m_Module.StartScale = 1.f;
	m_Module.EndScale = 2.f;

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
