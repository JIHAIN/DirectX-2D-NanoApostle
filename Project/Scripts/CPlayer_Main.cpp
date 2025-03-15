#include "pch.h"
#include "CPlayer_Main.h"
#include "CPlayer_Ai.h"

CPlayer_Main::CPlayer_Main()
    : CScript((UINT)SCRIPT_TYPE::PLAYER_MAIN)
    , pParticle(nullptr)
    , pParticle1(nullptr)
    , PostObj(nullptr)
    , m_Time(0.f)
{
}

CPlayer_Main::~CPlayer_Main()
{
}


void CPlayer_Main::Begin()
{
	SetAI(true);
}

void CPlayer_Main::SaveComponent(FILE* _File)
{
}

void CPlayer_Main::LoadComponent(FILE* _File)
{
}


void CPlayer_Main::createPost()
{
    PostObj = new CGameObject;
    PostObj->SetName(L"FadeOut");
    PostObj->AddComponent(new CMeshRender);

    PostObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
    PostObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    PostObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"FadeInEffectMtrl"));
    PostObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
    CreateObject(PostObj, 4, false);
}

void CPlayer_Main::AI_In()
{
    pParticle = nullptr;
    pParticle1 = nullptr;
    PostObj = nullptr;
    m_Time = 0.f;

	FlipbookPlayer()->Play((UINT)Player_Ani_State::Warp, 6.f, false);
    createPost();

    pParticle = new CGameObject;
    pParticle->SetName(L"Warp_Particle");
    pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Gradient_2Side_Horiz.png")));
    pParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
    CreateObject(pParticle, 2, false);

    pParticle1 = new CGameObject;
    pParticle1->SetName(L"Warp_Particle1");
    pParticle1->AddComponent(new CParticleSystem(ParticleSetting1(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Glow10_FullyWhite.png")));
    pParticle1->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
    CreateObject(pParticle1, 2, false);
}


void CPlayer_Main::AI_Tick()
{
    m_Time += DT;

    // 진행도 계산 (0 ~ 1)
    float progress = m_Time / 3.f;

    // 포스트 프로세스 효과에 진행도 전달
    if (PostObj && PostObj->MeshRender())
    {
        PostObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, progress);
    }



    // 애니메이션이 끝나면 기본 상태로
    if (FlipbookPlayer()->isFinish())
    {
        P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
        return;
    }
}

void CPlayer_Main::AI_Exit()
{
    if (pParticle != nullptr)
        DestroyObject(pParticle);
    if (pParticle1 != nullptr)
        DestroyObject(pParticle1);
    if (PostObj != nullptr)
        DestroyObject(PostObj);
}

tParticleModule CPlayer_Main::ParticleSetting()
{
    // 파티클 기능(모듈) 정보 세팅
            // Spawn Module
    tParticleModule m_Module;

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(0.6f, 0.6f, 0.6f, 0.05f);
    m_Module.MinLife = 3.f;
    m_Module.MaxLife = 3.f;
    m_Module.vSpawnMinScale = Vec3(100.f, 120.f, 100.f);
    m_Module.vSpawnMaxScale = Vec3(100.f, 120.f, 100.f);

    m_Module.SpawnShape = 0;
    m_Module.SpawnShapeScale.x = 1;
    m_Module.SpawnShapeScale.y = 1;

    m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
    m_Module.BlockSpawnShapeScale.x = 1;
    m_Module.BlockSpawnShapeScale.y = 1;

    m_Module.SpaceType = 0; // Local Space 

    // AddVelocity Module
    m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
    m_Module.AddVelocityType = 1;
    m_Module.AddVelocityFixedDir = Vec3(0.1f, 0.1f, 0.1f); // 이걸 내 방향 기준으로
    m_Module.AddMinSpeed = 1.f;
    m_Module.AddMaxSpeed = 1.f;

    // Render Module
    m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
    m_Module.EndColor = Vec4(0.5f, 0.5f, 0.6f, 0.01f);
    m_Module.FadeOut = true;
    m_Module.FadeOutStartRatio = 1.f;
    m_Module.VelocityAlignment = false;

    // SPAWN BURST
    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
    m_Module.SpawnBurstCount = 1.f;
    m_Module.SpawnBurstRepeat = false;
    m_Module.SpawnBurstRepeatTime = 3.f;

    // Scale Module
    m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
    m_Module.StartScale = 1.f;
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

tParticleModule CPlayer_Main::ParticleSetting1()
{
    // 파티클 기능(모듈) 정보 세팅
               // Spawn Module
    tParticleModule m_Module;

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
    m_Module.SpawnRate = 20;
    m_Module.vSpawnColor = Vec4(0.5f, 0.6f, 0.6f, 0.1f);
    m_Module.MinLife = 0.f;
    m_Module.MaxLife = 1.f;
    m_Module.vSpawnMinScale = Vec3(2.f, 2.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(20.f, 20.f, 1.f);

    m_Module.SpawnShape = 1;
    m_Module.SpawnShapeScale.x = 100;
    m_Module.SpawnShapeScale.y = 100;

    m_Module.BlockSpawnShape = 1; // 생성될 지형의 제한 
    m_Module.BlockSpawnShapeScale.x = 50;
    m_Module.BlockSpawnShapeScale.y = 50;

    m_Module.SpaceType = 0; // Local Space 

    // AddVelocity Module
    m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
    m_Module.AddVelocityType = 1;
    m_Module.AddVelocityFixedDir = Vec3(0.1f, 0.1f, 0.1f); // 이걸 내 방향 기준으로
    m_Module.AddMinSpeed = 100.f;
    m_Module.AddMaxSpeed = 300.f;

    // Render Module
    m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
    m_Module.EndColor = Vec4(0.2f, 0.5f, 0.6f, 0.01f);
    m_Module.FadeOut = true;
    m_Module.FadeOutStartRatio = 1.f;
    m_Module.VelocityAlignment = false;

    // SPAWN BURST
    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
    m_Module.SpawnBurstCount = 1.f;
    m_Module.SpawnBurstRepeat = false;
    m_Module.SpawnBurstRepeatTime = 3.f;

    // Scale Module
    m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
    m_Module.StartScale = 3.f;
    m_Module.EndScale = 1.f;

    // Drag Module (감속)
    m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
    m_Module.DestNormalizedAge = 1.f;
    m_Module.LimitSpeed = 2.f;

    // Noise Force Module
    m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
    m_Module.NoiseForceTerm = 0.3f;
    m_Module.NoiseForceScale = 100.f;

    return m_Module;
}
