#include "pch.h"
#include "CPlayer_Dash.h"
#include "CPlayerScript.h"
#include "CPlayer_Ai.h"


CPlayer_Dash::CPlayer_Dash()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_DASH)
	, PlayerScript(nullptr)
    , pParticle(nullptr)
	, m_DashStartPos(0.f, 0.f)
	, m_DashTargetPos(0.f, 0.f)
	, m_DashDirection(0.f, 0.f)
	, m_DashProgress(0.f)
	, m_IsDashing(false)
    , m_CooldownTimer(0.f)
{

	SetAI(true);
}

CPlayer_Dash::~CPlayer_Dash()
{
}

void CPlayer_Dash::Begin()
{
    SetAI(true);

	PlayerScript = (CPlayerScript*)GetOwner()->GetScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);
    pParticle = nullptr;
    m_DashStartPos = Vec2(0.f, 0.f);
    m_DashTargetPos = Vec2(0.f, 0.f);
    m_DashDirection = Vec2(0.f, 0.f);
    m_DashProgress = 0.f;
    m_CooldownTimer = 0.f;
    m_IsDashing = false;
}

void CPlayer_Dash::Tick()
{

}

void CPlayer_Dash::AI_In()
{
    m_IsDashing = true;
    m_DashProgress = 0.f;

    // 상태에 따른 대시 방향 설정
    CPlayer_Ai* PlayerAiScript = GETPAI();
    CPlayer_Dash_State dashState = PlayerAiScript->Get_Player_Dash_State();

    float AniTime = 10.f;

    switch (dashState)
    {
    case CPlayer_Dash_State::Left:
        m_DashDirection = Vec2(-1.f, 0.f);
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Right, AniTime + 10.f, false);
        break;
    case CPlayer_Dash_State::Right:
        m_DashDirection = Vec2(1.f, 0.f);
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Right, AniTime + 10.f, false);
        break;
    case CPlayer_Dash_State::Up:
        m_DashDirection = Vec2(0.f, 1.f);
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Up, AniTime, false);
        break;
    case CPlayer_Dash_State::LeftUP:
        m_DashDirection = Vec2(-0.707f, 0.707f);  // 대각선 정규화된 값
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Up, AniTime, false);
        break;
    case CPlayer_Dash_State::RightUP:
        m_DashDirection = Vec2(0.707f, 0.707f);   // 대각선 정규화된 값
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Up, AniTime, false);
        break;
    case CPlayer_Dash_State::Down:
        m_DashDirection = Vec2(0.f, -1.f);
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Down, AniTime, false);
        break;
    case CPlayer_Dash_State::LeftDOWN:
        m_DashDirection = Vec2(-0.707f, -0.707f);  // 대각선 정규화된 값
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Down, AniTime, false);
        break;
    case CPlayer_Dash_State::RightDOWN:
        m_DashDirection = Vec2(0.707f, -0.707f);   // 대각선 정규화된 값
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Dash_Down, AniTime, false);
        break;
    default:
        return;  // 유효하지 않은 상태면 대시하지 않음
    }

    pParticle = new CGameObject;
    pParticle->SetName(L"Dash_Particle");
    pParticle->AddComponent(new CParticleSystem(ParticleSetting(Vec3(-m_DashDirection.x, -m_DashDirection.y, 0.f)), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\FeelSnakeFood.png")));
    pParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
    CreateObject(pParticle, 2, false);     
    
    // 초기화
    GetOwner()->RigidBody()->SetVelocity(Vec2(0.f, 0.f));
    GetOwner()->RigidBody()->SetfrictDuration(0.1f);

    PlayerAiScript->ActivateInvincible();

    // Sound 로딩하기
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\PlayerDodge.wav", L"Sound\\PlayerDodge.wav");
    pSound->Play(1, 0.9f, true);
}

void CPlayer_Dash::AI_Tick()
{
    if (FlipbookPlayer()->isFinish())
    {
        P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
        return;
    }

    m_DashProgress += DT / DASH_DURATION;
    float halfPoint = 0.4f;

    Vec3 m_PlayerCurPos = Transform()->GetRelativePos();

    if (m_DashProgress <= halfPoint && m_IsDashing)
    {
        // 전반부: 매우 강한 가속 (현재와 동일)
        float dashForce = 200.f;
        float accelerationCurve = powf(1.f - (m_DashProgress / halfPoint), 0.5f);
        RigidBody()->AddForce(m_DashDirection * dashForce * accelerationCurve);
        RigidBody()->SetfrictDuration(0.1f);
    }
    else
    {
        // 후반부: 더 급격한 감속
        float decelerationProgress = (m_DashProgress - halfPoint) / halfPoint; // 0~1

        // 지수함수적 감속 커브 (더 급격한 감속)
        float decelerationCurve = powf(decelerationProgress, 2.f) * 3.f;

        // 마찰력 급격하게 증가
        RigidBody()->SetfrictDuration(2.0f + decelerationCurve);

        // 추가적인 역방향 힘 적용 (더 확실한 정지를 위해)
        if (decelerationProgress > 0.5f)  // 감속 후반부에
        {
           
            RigidBody()->SetVelocity(Vec2(0.f, 0.f));

            const bool* Player_Move = PlayerScript->Get_Player_Move_limit();
           

            float m_PlayerSpeed = 200.f;

            if (KEY_PRESSED(KEY::A) && Player_Move[0])
            {
                m_PlayerCurPos.x -= m_PlayerSpeed * DT;
                Transform()->SetRelativePos(m_PlayerCurPos);
            }
            if (KEY_PRESSED(KEY::D) && Player_Move[1])
            {
                m_PlayerCurPos.x += m_PlayerSpeed * DT;
                Transform()->SetRelativePos(m_PlayerCurPos);

            }
            if (KEY_PRESSED(KEY::W) && Player_Move[2])
            {
                m_PlayerCurPos.y += m_PlayerSpeed * DT;
                Transform()->SetRelativePos(m_PlayerCurPos);

            }
            if (KEY_PRESSED(KEY::S) && Player_Move[3])
            {
                m_PlayerCurPos.y -= m_PlayerSpeed * DT;
                Transform()->SetRelativePos(m_PlayerCurPos);

            }

        }
    }

    pParticle->Transform()->SetRelativePos(m_PlayerCurPos);
}
void CPlayer_Dash::AI_Exit()
{
    m_IsDashing = false;
    GetOwner()->RigidBody()->SetfrictDuration(0.1f);

    if(pParticle != nullptr)
        DestroyObject(pParticle);

}
void CPlayer_Dash::SaveComponent(FILE* _File)
{
}

void CPlayer_Dash::LoadComponent(FILE* _File)
{
}

void CPlayer_Dash::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (!m_IsDashing || _OtherObject->GetScript((UINT)SCRIPT_TYPE::WALLSCRIPT) == nullptr)
        return;

    // 현재 속도 가져오기
    Vec2 currentVelocity = RigidBody()->GetVelocity();

    // 벽 콜라이더 정보
    Vec2 vWallPos = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x,
        _OtherCollider->GetColliderWorldMat().Translation().y);
    Vec2 vMyPos = Vec2(_Collider->GetColliderWorldMat().Translation().x,
        _Collider->GetColliderWorldMat().Translation().y);

    // 대시 방향에 따른 속도 조정
    if (m_DashDirection.x > 0 && vMyPos.x < vWallPos.x) // 오른쪽으로 대시
    {
        currentVelocity.x = 0.f;
    }
    else if (m_DashDirection.x < 0 && vMyPos.x > vWallPos.x) // 왼쪽으로 대시
    {
        currentVelocity.x = 0.f;
    }
    else if (m_DashDirection.y > 0 && vMyPos.y < vWallPos.y) // 위로 대시
    {
        currentVelocity.y = 0.f;
    }
    else if (m_DashDirection.y < 0 && vMyPos.y > vWallPos.y) // 아래로 대시
    {
        currentVelocity.y = 0.f;
    }

    // 수정된 속도 적용
    RigidBody()->SetVelocity(currentVelocity);
    m_IsDashing = false;
}

void CPlayer_Dash::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (!m_IsDashing || _OtherObject->GetScript((UINT)SCRIPT_TYPE::WALLSCRIPT) == nullptr)
        return;

    // 현재 속도 가져오기
    Vec2 currentVelocity = RigidBody()->GetVelocity();

    // 벽 콜라이더 정보
    Vec2 vWallPos = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x,
        _OtherCollider->GetColliderWorldMat().Translation().y);
    Vec2 vMyPos = Vec2(_Collider->GetColliderWorldMat().Translation().x,
        _Collider->GetColliderWorldMat().Translation().y);

    // 대시 방향에 따른 속도 조정
    if (m_DashDirection.x > 0 && vMyPos.x < vWallPos.x) // 오른쪽으로 대시
    {
        currentVelocity.x = 0.f;
    }
    else if (m_DashDirection.x < 0 && vMyPos.x > vWallPos.x) // 왼쪽으로 대시
    {
        currentVelocity.x = 0.f;
    }
    else if (m_DashDirection.y > 0 && vMyPos.y < vWallPos.y) // 위로 대시
    {
        currentVelocity.y = 0.f;
    }
    else if (m_DashDirection.y < 0 && vMyPos.y > vWallPos.y) // 아래로 대시
    {
        currentVelocity.y = 0.f;
    }

    // 수정된 속도 적용
    RigidBody()->SetVelocity(currentVelocity);
    m_IsDashing = false;
}

Vec2 CPlayer_Dash::Lerp(const Vec2& start, const Vec2& end, float t)
{
	return start + (end - start) * t;
}

tParticleModule CPlayer_Dash::ParticleSetting(Vec3 _Dir)
{

    Vec2 tSpawnShapeScale = Vec2(0.f, 0.f); // Box

    if (_Dir.x != 0.f && _Dir.y == 0.f) // Left , Right
    {
        tSpawnShapeScale.x = 300.f;
        tSpawnShapeScale.y = 90.f;
    }
    else if (_Dir.x == 0.f && _Dir.y != 0.f) // Up , Down
    {
        tSpawnShapeScale.x = 90.f;
        tSpawnShapeScale.y = 300.f;
    }
    else // LU, LD, RU, RD
    {
        tSpawnShapeScale.x = 200.f;
        tSpawnShapeScale.y = 200.f;
    }

    // 파티클 기능(모듈) 정보 세팅
            // Spawn Module
    tParticleModule m_Module;

    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
    m_Module.SpawnRate = 0;
    m_Module.vSpawnColor = Vec4(0.7f, 0.7f, 0.7f, 0.7f);
    m_Module.MinLife = 0.5f;
    m_Module.MaxLife = 0.8f;
    m_Module.vSpawnMinScale = Vec3(5.f, 5.f, 1.f);
    m_Module.vSpawnMaxScale = Vec3(10.f, 10.f, 1.f);

    m_Module.SpawnShape = 0;
    m_Module.SpawnShapeScale.x = tSpawnShapeScale.x;
    m_Module.SpawnShapeScale.y = tSpawnShapeScale.y;

    m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
    m_Module.BlockSpawnShapeScale.x = tSpawnShapeScale.x;
    m_Module.BlockSpawnShapeScale.y = tSpawnShapeScale.y;

    m_Module.SpaceType = 0; // Local Space 

    // AddVelocity Module
    m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
    m_Module.AddVelocityType = 3;
    m_Module.AddVelocityFixedDir = _Dir; // 이걸 내 방향 기준으로
    m_Module.AddMinSpeed = 300.f;
    m_Module.AddMaxSpeed = 510.f;

    // Render Module
    m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
    m_Module.EndColor = Vec4(0.7f, 0.7f, 0.7f, 0.3f);
    m_Module.FadeOut = true;
    m_Module.FadeOutStartRatio = 0.01f;
    m_Module.VelocityAlignment = true;

    // SPAWN BURST
    m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
    m_Module.SpawnBurstCount = 15.f;
    m_Module.SpawnBurstRepeat = false;
    m_Module.SpawnBurstRepeatTime = 3.f;

    // Scale Module
    m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
    m_Module.StartScale = 3.f;
    m_Module.EndScale = 0.7f;

    // Drag Module (감속)
    m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
    m_Module.DestNormalizedAge = 1.f;
    m_Module.LimitSpeed = 2.f;

    // Noise Force Module
    m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
    m_Module.NoiseForceTerm = 0.3f;
    m_Module.NoiseForceScale = 600.f;

    return m_Module;
}
