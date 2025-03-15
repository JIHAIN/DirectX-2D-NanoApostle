#include "pch.h"
#include "CGroundBoundary.h"


CGroundBoundary::CGroundBoundary()
    : CScript((UINT)SCRIPT_TYPE::GROUNDBOUNDARY)
    , m_CanExit()
{
}

CGroundBoundary::~CGroundBoundary()
{
}


void CGroundBoundary::Begin()
{
    m_CanExit[0,1,2,3] = false;
}

void CGroundBoundary::Tick()
{
}

void CGroundBoundary::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CGroundBoundary::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

    // 플레이어의 발밑 체크용 콜라이더와의 충돌만 처리
    if (_OtherObject->GetName() != L"Player")
        return;

    // Ground의 콜라이더 정보
    Vec2 groundPos = Vec2(_Collider->GetColliderWorldMat().Translation().x,
        _Collider->GetColliderWorldMat().Translation().y);
    Vec2 groundScale = Vec2(_Collider->Transform()->GetWorldScale().x / 2.f,
        _Collider->Transform()->GetWorldScale().y / 2.f);

    // 플레이어 발밑 콜라이더 정보
    Vec2 playerPos = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x,
        _OtherCollider->GetColliderWorldMat().Translation().y);
    Vec2 playerScale = Vec2(_OtherCollider->Transform()->GetWorldScale().x / 2.f,
        _OtherCollider->Transform()->GetWorldScale().y / 2.f);

    // Ground의 경계
    float groundLeft = groundPos.x - groundScale.x;
    float groundRight = groundPos.x + groundScale.x;
    float groundTop = groundPos.y + groundScale.y;
    float groundBottom = groundPos.y - groundScale.y;

    Vec3 adjustedPos = _OtherObject->Transform()->GetRelativePos();

    // 각 방향별로 나갈 수 없는 경우에만 위치 제한
    if (!m_CanExit[0] && (playerPos.x - playerScale.x) < groundLeft)
    {
        adjustedPos.x = groundLeft + playerScale.x;
    }
    if (!m_CanExit[1] && (playerPos.x + playerScale.x) > groundRight)
    {
        adjustedPos.x = groundRight - playerScale.x;
    }
    if (!m_CanExit[2] && (playerPos.y + playerScale.y) > groundTop)
    {
        adjustedPos.y = groundTop - playerScale.y;
    }
    if (!m_CanExit[3] && (playerPos.y - playerScale.y) < groundBottom)
    {
        adjustedPos.y = groundBottom + playerScale.y;
    }

    // 플레이어 발밑 콜라이더의 부모(플레이어)의 위치 조정
    // _OtherObject->GetParent()->Transform()->SetRelativePos(adjustedPos);
    _OtherObject->Transform()->SetRelativePos(adjustedPos);

    // 만약 대시 중이고 경계에 막혔다면 대시 중단
    //if (adjustedPos != _OtherObject->GetParent()->Transform()->GetRelativePos())
    //{
    //    CPlayer_Dash* dashScript = (CPlayer_Dash*)_OtherObject->GetParent()->GetScript((UINT)SCRIPT_TYPE::PLAYER_DASH);
    //    if (dashScript && dashScript->IsDashing())
    //    {
    //        dashScript->StopDash();
    //        _OtherObject->GetParent()->RigidBody()->SetVelocity(Vec2(0.f, 0.f));
    //    }
    //}
}


void CGroundBoundary::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CGroundBoundary::SaveComponent(FILE* _File)
{
}

void CGroundBoundary::LoadComponent(FILE* _File)
{
}
