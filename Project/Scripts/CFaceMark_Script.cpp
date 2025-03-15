#include "pch.h"

#include "CFaceMark_Script.h"
#include "CPlayer_Ai.h"
#include <Engine/CKeyMgr.h>

CFaceMark_Script::CFaceMark_Script()
    : CScript((UINT)SCRIPT_TYPE::FACEMARK_SCRIPT)
    , m_Player(nullptr)
    , m_PlayerScript(nullptr)
{
}

CFaceMark_Script::~CFaceMark_Script()
{
}

void CFaceMark_Script::Begin()
{
    m_Player = GetOwner()->GetParent();
    m_PlayerScript = (CPlayer_Ai*)m_Player->GetScript((UINT)SCRIPT_TYPE::PLAYER_AI);
}

float CFaceMark_Script::GetAngle8Dir(const Vec3& dir, float _playerRotationY)
{
    CPlayer_Dash_State dashState = m_PlayerScript->Get_Player_Dash_State();
    Vec3 playerScale = m_Player->Transform()->GetRelativeScale();
    bool isFlipped = (playerScale.x < 0.f);

    float angle;
    // 왼손 좌표계에서 각도는 시계 방향이 양수
    switch (dashState)
    {
    case CPlayer_Dash_State::Right:
        angle = -XM_PI * 0.5f;       // 오른쪽 (-90도)
        break;
    case CPlayer_Dash_State::Left:
        angle = XM_PI * 0.5f;        // 왼쪽 (90도)
        break;
    case CPlayer_Dash_State::Up:
        angle = 0.0f;                // 위 (0도, 기본 이미지 방향)
        break;
    case CPlayer_Dash_State::Down:
        angle = XM_PI;               // 아래 (180도)
        break;
    case CPlayer_Dash_State::RightUP:
        angle = -XM_PI * 0.25f;      // 오른쪽 위 (-45도)
        break;
    case CPlayer_Dash_State::RightDOWN:
        angle = -XM_PI * 0.75f;      // 오른쪽 아래 (-135도)
        break;
    case CPlayer_Dash_State::LeftUP:
        angle = XM_PI * 0.25f;       // 왼쪽 위 (45도)
        break;
    case CPlayer_Dash_State::LeftDOWN:
        angle = XM_PI * 0.75f;       // 왼쪽 아래 (135도)
        break;
    default:
        angle = 0.0f;
        break;
    }

    // 플레이어 반전 시 각도도 반전
    if (isFlipped)
    {
        if (angle != 0.f && angle != XM_PI)  // 위/아래 방향이 아닐 때만
        {
            angle = -angle;
        }
    }

    return angle;
}

void CFaceMark_Script::Tick()
{
    Vec3 pRota = Transform()->GetRelativeRotation();
    Vec3 pScale = Transform()->GetRelativeScale();
    Vec3 playerScale = m_Player->Transform()->GetRelativeScale();

    // 화살표 회전 각도 계산
    pRota.z = GetAngle8Dir(m_PlayerScript->Get_Player_Dir(), 0.f);

    // 스케일 처리 (부모 오브젝트의 스케일 반전 따라가기)
    if (playerScale.x < 0.f)
    {
        // 부모가 반전되었을 때
        pScale.x = -abs(pScale.x);
    }
    else
    {
        // 부모가 정방향일 때
        pScale.x = abs(pScale.x);
    }

    // 마우스 조준 로직
    if (KEY_TAP(KEY::LBTN))
    {
        Vec2 MousePos = g_MouseInfo.MousePos;
        Vec3 pWPos = GetOwner()->Transform()->GetRelativePos();
        Vec2 PlayerMouseDir = MousePos - Vec2(pWPos.x, pWPos.y);
        PlayerMouseDir.Normalize();
        float angle = atan2(PlayerMouseDir.y, PlayerMouseDir.x);
        angle = angle - (XM_PI / 2.f);

        // 마우스 조준시에도 플레이어 반전 고려
        if (playerScale.x < 0.f)
        {
            angle = XM_PI - angle;
        }
        pRota.z = angle;
    }

    Transform()->SetRelativeScale(pScale);
    Transform()->SetRelativeRotation(pRota);
}

void CFaceMark_Script::SaveComponent(FILE* _File)
{
}

void CFaceMark_Script::LoadComponent(FILE* _File)
{
}