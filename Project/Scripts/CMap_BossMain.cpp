#include "pch.h"
#include "CMap_BossMain.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CMap_BossMain::CMap_BossMain()
    : CScript((UINT)SCRIPT_TYPE::MAP_BOSSMAIN)
    , pParticle1(nullptr)
    , pParticle2(nullptr)
    , m_Time(0.f)
    , m_WarpOn(false)
{
}

CMap_BossMain::~CMap_BossMain()
{
    if (pParticle1 != nullptr)
        DestroyObject(pParticle1);
    if (pParticle2 != nullptr)
        DestroyObject(pParticle2);
    if (PostObj != nullptr)
        DestroyObject(PostObj);
    
}


void CMap_BossMain::createPost()
{
    PostObj = new CGameObject;
    PostObj->SetName(L"FadeOut");
    PostObj->AddComponent(new CMeshRender);

    PostObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
    PostObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    PostObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"FadeOutEffectMtrl"));
    PostObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
    CreateObject(PostObj, 4, false);
}

void CMap_BossMain::Tick()
{
    if (m_WarpOn)
    {
        m_Time += DT;

        // 진행도 계산 (0 ~ 1)
        float progress = m_Time / 2.f;

        // 포스트 프로세스 효과에 진행도 전달
        if (PostObj && PostObj->MeshRender())
        {
            PostObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, progress);
        }

        if (m_Time >= 2.f)
        {
            wstring CurName = L"Boss";
            CLevelMgr::GetInst()->RequestLevelLoad(CurName);
        }
    }
}

void CMap_BossMain::Begin()
{

    m_Time = 0.f;
    m_WarpOn = false;

    pParticle1 = new CGameObject;
    pParticle1->SetName(L"MainBoss_Particle1");
    pParticle1->AddComponent(new CParticleSystem(ParticleSetting1(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\TrainingPlatform_Diamond.png")));
    pParticle1->Transform()->SetRelativePos(Vec3(0.f, -33.f, 11.f));
    CreateObject(pParticle1, 2, false);

    pParticle2 = new CGameObject;
    pParticle2->SetName(L"MainBoss_Particle2");
    pParticle2->AddComponent(new CParticleSystem(ParticleSetting2(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Training_Ring.png")));
    pParticle2->Transform()->SetRelativePos(Vec3(0.f, -33.f, 11.f));
    CreateObject(pParticle2, 2, false);
}

void CMap_BossMain::SaveComponent(FILE* _File)
{
}

void CMap_BossMain::LoadComponent(FILE* _File)
{
}

void CMap_BossMain::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMap_BossMain::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetName() == L"Player" && KEY_TAP(KEY::E))
    {
        m_WarpOn = true;

        createPost();
    }
}

void CMap_BossMain::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

tParticleModule CMap_BossMain::ParticleSetting1()
{
    // 파티클 기능(모듈) 정보 세팅
                  // Spawn Module
    tParticleModule m_Module;

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(0.5f, 0.6f, 0.6f, 0.1f);
    m_Module.MinLife = 0.3f;
    m_Module.MaxLife = 0.3f;
    m_Module.vSpawnMinScale = Vec3(90.f, 50.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(90.f, 50.f, 1.f);

    m_Module.SpawnShape = 1;
    m_Module.SpawnShapeScale.x = 1;
    m_Module.SpawnShapeScale.y = 1;

    m_Module.BlockSpawnShape = 1; // 생성될 지형의 제한 
    m_Module.BlockSpawnShapeScale.x = 1;
    m_Module.BlockSpawnShapeScale.y = 1;

    m_Module.SpaceType = 0; // Local Space 

    // AddVelocity Module
    m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
    m_Module.AddVelocityType = 3;
    m_Module.AddVelocityFixedDir = Vec3(0.f, 0.1f, 0.f); // 이걸 내 방향 기준으로
    m_Module.AddMinSpeed = 10.f;
    m_Module.AddMaxSpeed = 50.f;

    // Render Module
    m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
    m_Module.EndColor = Vec4(0.2f, 0.5f, 0.6f, 0.1f);
    m_Module.FadeOut = true;
    m_Module.FadeOutStartRatio = 1.f;
    m_Module.VelocityAlignment = false;

    // SPAWN BURST
    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
    m_Module.SpawnBurstCount = 1.f;
    m_Module.SpawnBurstRepeat = true;
    m_Module.SpawnBurstRepeatTime = 0.6f;

    // Scale Module
    m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = false;
    m_Module.StartScale = 3.f;
    m_Module.EndScale = 1.f;

    // Drag Module (감속)
    m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
    m_Module.DestNormalizedAge = 1.f;
    m_Module.LimitSpeed = 2.f;

    // Noise Force Module
    m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
    m_Module.NoiseForceTerm = 0.3f;
    m_Module.NoiseForceScale = 100.f;

    return m_Module;
}

tParticleModule CMap_BossMain::ParticleSetting2()
{
    // 파티클 기능(모듈) 정보 세팅
                 // Spawn Module
    tParticleModule m_Module;

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(0.5f, 0.6f, 0.6f, 0.1f);
    m_Module.MinLife = 0.7f;
    m_Module.MaxLife = 0.7f;
    m_Module.vSpawnMinScale = Vec3(320.f, 200.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(320.f, 200.f, 1.f);

    m_Module.SpawnShape = 1;
    m_Module.SpawnShapeScale.x = 1;
    m_Module.SpawnShapeScale.y = 1;

    m_Module.BlockSpawnShape = 1; // 생성될 지형의 제한 
    m_Module.BlockSpawnShapeScale.x = 1;
    m_Module.BlockSpawnShapeScale.y = 1;

    m_Module.SpaceType = 0; // Local Space 

    // AddVelocity Module
    m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
    m_Module.AddVelocityType = 3;
    m_Module.AddVelocityFixedDir = Vec3(0.f, 0.1f, 0.f); // 이걸 내 방향 기준으로
    m_Module.AddMinSpeed = 0.1f;
    m_Module.AddMaxSpeed = 0.1f;

    // Render Module
    m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
    m_Module.EndColor = Vec4(0.2f, 0.5f, 0.6f, 0.1f);
    m_Module.FadeOut = true;
    m_Module.FadeOutStartRatio = 1.f;
    m_Module.VelocityAlignment = false;

    // SPAWN BURST
    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
    m_Module.SpawnBurstCount = 1.f;
    m_Module.SpawnBurstRepeat = true;
    m_Module.SpawnBurstRepeatTime = 0.9f;

    // Scale Module
    m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = false;
    m_Module.StartScale = 3.f;
    m_Module.EndScale = 1.f;

    // Drag Module (감속)
    m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
    m_Module.DestNormalizedAge = 1.f;
    m_Module.LimitSpeed = 2.f;

    // Noise Force Module
    m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
    m_Module.NoiseForceTerm = 0.3f;
    m_Module.NoiseForceScale = 100.f;

    return m_Module;
}



