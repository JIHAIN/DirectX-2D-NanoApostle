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
    // �޼� ��ǥ�迡�� ������ �ð� ������ ���
    switch (dashState)
    {
    case CPlayer_Dash_State::Right:
        angle = -XM_PI * 0.5f;       // ������ (-90��)
        break;
    case CPlayer_Dash_State::Left:
        angle = XM_PI * 0.5f;        // ���� (90��)
        break;
    case CPlayer_Dash_State::Up:
        angle = 0.0f;                // �� (0��, �⺻ �̹��� ����)
        break;
    case CPlayer_Dash_State::Down:
        angle = XM_PI;               // �Ʒ� (180��)
        break;
    case CPlayer_Dash_State::RightUP:
        angle = -XM_PI * 0.25f;      // ������ �� (-45��)
        break;
    case CPlayer_Dash_State::RightDOWN:
        angle = -XM_PI * 0.75f;      // ������ �Ʒ� (-135��)
        break;
    case CPlayer_Dash_State::LeftUP:
        angle = XM_PI * 0.25f;       // ���� �� (45��)
        break;
    case CPlayer_Dash_State::LeftDOWN:
        angle = XM_PI * 0.75f;       // ���� �Ʒ� (135��)
        break;
    default:
        angle = 0.0f;
        break;
    }

    // �÷��̾� ���� �� ������ ����
    if (isFlipped)
    {
        if (angle != 0.f && angle != XM_PI)  // ��/�Ʒ� ������ �ƴ� ����
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

    // ȭ��ǥ ȸ�� ���� ���
    pRota.z = GetAngle8Dir(m_PlayerScript->Get_Player_Dir(), 0.f);

    // ������ ó�� (�θ� ������Ʈ�� ������ ���� ���󰡱�)
    if (playerScale.x < 0.f)
    {
        // �θ� �����Ǿ��� ��
        pScale.x = -abs(pScale.x);
    }
    else
    {
        // �θ� �������� ��
        pScale.x = abs(pScale.x);
    }

    // ���콺 ���� ����
    if (KEY_TAP(KEY::LBTN))
    {
        Vec2 MousePos = g_MouseInfo.MousePos;
        Vec3 pWPos = GetOwner()->Transform()->GetRelativePos();
        Vec2 PlayerMouseDir = MousePos - Vec2(pWPos.x, pWPos.y);
        PlayerMouseDir.Normalize();
        float angle = atan2(PlayerMouseDir.y, PlayerMouseDir.x);
        angle = angle - (XM_PI / 2.f);

        // ���콺 ���ؽÿ��� �÷��̾� ���� ���
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