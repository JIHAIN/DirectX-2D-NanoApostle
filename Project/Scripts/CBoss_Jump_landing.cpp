#include "pch.h"
#include "CBoss_Jump_landing.h"
#include "CBoss_Ai.h"

CBoss_Jump_landing::CBoss_Jump_landing()
	: CScript((UINT)SCRIPT_TYPE::BOSS_JUMP_LANDING)
    , pParticle(nullptr)
{
}

CBoss_Jump_landing::~CBoss_Jump_landing()
{
}


tParticleModule CBoss_Jump_landing::ParticleSetting()
{
    tParticleModule m_Module = {};

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(1.f, 0.f, 0.f, 0.6f);
    m_Module.MinLife = 1.f;
    m_Module.MaxLife = 1.f;
    m_Module.vSpawnMinScale = Vec3(300.f, 70.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(300.f, 70.f, 1.f);

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
    m_Module.AddVelocityFixedDir = Vec3(0.f, 0.1f, 0); // 이걸 내 방향 기준으로
    m_Module.AddMinSpeed = 0.001;
    m_Module.AddMaxSpeed = 0.001;

    // Render Module
    m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
    m_Module.EndColor = Vec4(0.6f, 0.f, 0.f, 0.01f);
    m_Module.FadeOut = false;
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


void CBoss_Jump_landing::Begin()
{
	SetAI(true);
}

void CBoss_Jump_landing::AI_In()
{
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Jump_Atk_landing, 20.f, false);

    Vec3 Pos = Transform()->GetRelativePos();

    pParticle = new CGameObject;
    pParticle->SetName(L"Jumplanding_Particle");
    pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\GroundCrack_Small_0.png")));
    pParticle->Transform()->SetRelativePos(Vec3(Pos.x, Pos.y - 200.f, Pos.z));
    CreateObject(pParticle, 2, false);


    // Sound 로딩하기
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\Grounding.wav", L"Sound\\Grounding.wav");
    pSound->Play(1, 0.3f, true);
}

void CBoss_Jump_landing::AI_Tick()
{
	if (FlipbookPlayer()->isFinish())
	{
        // 0 또는 1의 랜덤값 생성
        int randomValue = rand() % 2;

        if (randomValue == 0)
        {
            B_CHANGE(SCRIPT_TYPE::BOSS_CHARGE_ATK);
        }
        else
        {
            B_CHANGE(SCRIPT_TYPE::BOSS_BOTTOM_ATK);
        }		return;
	}
}

void CBoss_Jump_landing::AI_Exit()
{
    if (pParticle != nullptr)
        DestroyObject(pParticle);
}

void CBoss_Jump_landing::SaveComponent(FILE* _File)
{
}

void CBoss_Jump_landing::LoadComponent(FILE* _File)
{
}

void CBoss_Jump_landing::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Jump_landing::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Jump_landing::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


