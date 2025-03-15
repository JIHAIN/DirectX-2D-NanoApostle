#include "pch.h"
#include "CBoss_Jump_Atk.h"
#include "CBoss_Ai.h"

CBoss_Jump_Atk::CBoss_Jump_Atk()
	: CScript((UINT)SCRIPT_TYPE::BOSS_JUMP_ATK)
    , m_Jump_Obj(nullptr)
    , m_fCurrentTime(0.f)
    , m_vStartPos(0.f, 0.f, 0.f)
    , m_vTargetPos(0.f, 0.f, 0.f)
    , pParticle(nullptr)
    , pParticle1(nullptr)
    , pParticle2(nullptr)
    , m_JumpHeight(300.f)  // 점프 높이 설정
    , m_HoverTime(0.1f)      // 체공 시간
    , m_CurrentHoverTime(0.f)
    , m_Phase(JumpPhase::RISING)
{
}

CBoss_Jump_Atk::~CBoss_Jump_Atk()
{
}

void CBoss_Jump_Atk::Begin()
{
	SetAI(true);
    m_fCurrentTime = 0.f;
    m_vStartPos = Vec3(0.f, 0.f, 0.f);
    m_vTargetPos = Vec3(0.f, 0.f, 0.f);
    m_Jump_Obj = nullptr;
    m_JumpHeight = 400.f; // 점프 높이 설정
    m_HoverTime = 0.15f;     // 체공 시간
    m_CurrentHoverTime = 0.f;
    m_Phase = JumpPhase::RISING;
}

void CBoss_Jump_Atk::AI_In()
{
    m_Jump_Obj = nullptr;

    CBoss_Ai* TEMP_AI = (CBoss_Ai*)GetOwner()->GetScript((UINT)SCRIPT_TYPE::BOSS_AI);

    m_vStartPos = Transform()->GetWorldPos();
    m_vTargetPos = TEMP_AI->GetTargetPlayer()->Transform()->GetWorldPos();

    m_JumpHeight = 400.f; // 점프 높이 설정
    m_fCurrentTime = 0.f;
    m_HoverTime = 0.15f;     // 체공 시간


	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Jump_Atk, 13.f, false);
    FlipbookPlayer()->SetTerm(2, 0.2f);

    // Sound 로딩하기
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\Jump.wav", L"Sound\\Jump.wav");
    pSound->Play(1, 0.3f, true);
}

void CBoss_Jump_Atk::AI_Tick()
{
    static bool first = true;

    if (FlipbookPlayer()->GetCurIdx() == 5 && first)
    {
        first = false;
        CreateOnject();
        m_Phase = JumpPhase::RISING;
        m_fCurrentTime = 0.f;
        m_CurrentHoverTime = 0.f;
    }

    if (FlipbookPlayer()->GetCurIdx() >= 5)
    {
        Vec3 newPos;

        switch (m_Phase)
        {
        case JumpPhase::RISING:
        {
            m_fCurrentTime += DT;
            float riseTime = 0.2f;  // 상승 시간
            float t = m_fCurrentTime / riseTime;

            if (t >= 1.f)
            {
                // 상승 완료, 체공 단계로
                m_Phase = JumpPhase::HOVERING;
                newPos = m_vTargetPos;
                newPos.y += m_JumpHeight;
            }
            else
            {
                // 빠르게 상승
                float easedT = 1.f - (1.f - t) * (1.f - t); // 이징 적용
                newPos = Vec3::Lerp(m_vStartPos, m_vTargetPos, easedT);
                newPos.y += m_JumpHeight * easedT;
            }
        }
        break;

        case JumpPhase::HOVERING:
        {
            m_CurrentHoverTime += DT;
            newPos = m_vTargetPos;
            newPos.y += m_JumpHeight;

            if (m_CurrentHoverTime >= m_HoverTime)
            {
                m_Phase = JumpPhase::FALLING;
                m_fCurrentTime = 0.f;
            }
        }
        break;

        case JumpPhase::FALLING:
        {
            m_fCurrentTime += DT;
            float fallTime = 0.1f;  // 빠른 낙하 시간
            float t = m_fCurrentTime / fallTime;

            if (t >= 1.f)
            {
                B_CHANGE(SCRIPT_TYPE::BOSS_JUMP_LANDING);
                first = true;
                m_Phase = JumpPhase::RISING;
                newPos = m_vTargetPos;
            }
            else
            {
                // 급격한 낙하
                float easedT = t * t; // 가속도 적용
                Vec3 startPos = m_vTargetPos;
                startPos.y += m_JumpHeight;
                newPos = Vec3::Lerp(startPos, m_vTargetPos, easedT);
            }
        }
        break;
        }

        Transform()->SetRelativePos(newPos);
    }
}

void CBoss_Jump_Atk::AI_Exit()
{
    if (m_Jump_Obj != nullptr)
        DestroyObject(m_Jump_Obj);
    if (pParticle != nullptr)
        DestroyObject(pParticle);
    if (pParticle1 != nullptr)
        DestroyObject(pParticle1);
    if (pParticle2 != nullptr)
        DestroyObject(pParticle2);
}

tParticleModule CBoss_Jump_Atk::ParticleSetting()
{
    tParticleModule m_Module = {};

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.3f);
    m_Module.MinLife = 0.f;
    m_Module.MaxLife = 0.3f;
    m_Module.vSpawnMinScale = Vec3(300.f, 100.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(300.f, 100.f, 1.f);

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
    m_Module.EndColor = Vec4(0.4f, 0.4f, 0.4f, 0.2f);
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
    m_Module.StartScale = 1.f;
    m_Module.EndScale = 3.f;

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

tParticleModule CBoss_Jump_Atk::ParticleSetting1()
{
    tParticleModule m_Module = {};

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.3f);
    m_Module.MinLife = 0.f;
    m_Module.MaxLife = 1.f;
    m_Module.vSpawnMinScale = Vec3(300.f, 100.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(300.f, 100.f, 1.f);

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

tParticleModule CBoss_Jump_Atk::ParticleSetting2()
{
    tParticleModule m_Module = {};

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.3f);
    m_Module.MinLife = 0.0f;
    m_Module.MaxLife = 0.2f;
    m_Module.vSpawnMinScale = Vec3(90.f, 110.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(90.f, 110.f, 1.f);

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
    m_Module.EndColor = Vec4(1.f, 1.f, 1.f, 0.1f);
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

void CBoss_Jump_Atk::CreateOnject()
{
    Vec3 Pos = Transform()->GetRelativePos();

    m_Jump_Obj = new CGameObject;
    m_Jump_Obj->SetName(L"Hump_Obj");

    m_Jump_Obj->AddComponent(new CRepeatSprite);
    m_Jump_Obj->RepeatSprite()->SetSingleImageSize(80.f, 260.f);
    m_Jump_Obj->RepeatSprite()->SetRepeatCount(2, 1);
    m_Jump_Obj->RepeatSprite()->SetTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\JetFire.png"));
    m_Jump_Obj->RepeatSprite()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, 0.5f);
    m_Jump_Obj->Transform()->SetIndependentScale(true);
    m_Jump_Obj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));

    CreateObject(m_Jump_Obj, 0, false);
    AddChild(GetOwner(), m_Jump_Obj);


    pParticle = new CGameObject;
    pParticle->SetName(L"Jump_Particle");
    pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Roar.png")));
    pParticle->Transform()->SetRelativePos(Vec3(Pos.x, Pos.y - 80.f, Pos.z));
    CreateObject(pParticle, 2, false);

    pParticle1 = new CGameObject;
    pParticle1->SetName(L"Jump_Particle1");
    pParticle1->AddComponent(new CParticleSystem(ParticleSetting1(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\GroundCrack1_FullyWhite.png")));
    pParticle1->Transform()->SetRelativePos(Vec3(Pos.x, Pos.y-100.f, Pos.z));
    CreateObject(pParticle1, 2, false);

    pParticle2 = new CGameObject;
    pParticle2->SetName(L"Jump_Particle2");
    pParticle2->AddComponent(new CParticleSystem(ParticleSetting2(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Strike_0_1.png")));
    pParticle2->Transform()->SetRelativePos(Vec3(Pos.x, Pos.y, Pos.z));
    CreateObject(pParticle2, 2, false);
}

void CBoss_Jump_Atk::SaveComponent(FILE* _File)
{
}

void CBoss_Jump_Atk::LoadComponent(FILE* _File)
{
}

void CBoss_Jump_Atk::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Jump_Atk::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Jump_Atk::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


