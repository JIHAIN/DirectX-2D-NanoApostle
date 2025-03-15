#include "pch.h"
#include "CSircleObj.h"
#include <Engine/CLayer.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>


CSircleObj::CSircleObj()
    : CScript((UINT)SCRIPT_TYPE::SIRCLEOBJ)
    , m_Type(0)
    , m_LiveTime(15.f)
    , m_CurrentTime(0.f)
    , m_StartPos(Vec3(0.f, 0.f, 0.f))
    , m_GroupIndex(0)
    , m_PatternStartPos(Vec3(0.f, 0.f, 0.f))
    , m_PatternEndPos(Vec3(0.f, 0.f, 0.f))
    , m_bForward(true)
    , m_MoveSpeed(300.f)
    , m_CurrentWaypoint(0)
{
}

CSircleObj::~CSircleObj()
{
}

void CSircleObj::Begin()
{
	m_CurrentTime = 0.f;
	m_LiveTime = 25.f;  // 기본 생존시간
	m_StartPos = Transform()->GetRelativePos();
	m_CurrentWaypoint = 0;

	if (m_Type == 1)
	{
		// 웨이포인트 설정
		m_Waypoints.push_back(Vec3(1812.f, 154.f, 1.f));
		m_Waypoints.push_back(Vec3(2338.f, 154.f, 1.f));
		m_Waypoints.push_back(Vec3(2338.f, -416.f, 1.f));
		m_Waypoints.push_back(Vec3(1812.f, -416.f, 1.f));
	}
    else if (m_Type == 4)
    {
        Vec3 currentPos = Transform()->GetRelativePos();
        if (abs(currentPos.x - 388.f) < 1.f)  // 첫 번째 객체
        {
            SetPatternPoints(Vec3(388.f, 1500.f, 1.f), Vec3(388.f, 1750.f, 1.f));
        }
        else if (abs(currentPos.x - 1050.f) < 1.f)  // 두 번째 객체
        {
            SetPatternPoints(Vec3(1050.f, 1500.f, 1.f), Vec3(1790.f, 1500.f, 1.f));
        }
        else  // 세 번째 객체
        {
            SetPatternPoints(Vec3(2625.f, 1500.f, 1.f), Vec3(1790.f, 1500.f, 1.f));
        }
    }
}

void CSircleObj::SetPatternPoints(Vec3 _start, Vec3 _end)
{
    m_PatternStartPos = _start;
    m_PatternEndPos = _end;
    m_bForward = true;
    m_MoveSpeed = 300.f;  // 이동 속도 설정
}

void CSircleObj::Tick()
{
    // 공통 회전 효과
    Vec3 Rota = Transform()->GetRelativeRotation();
    if (Rota.z >= XM_PI * 2)
        Rota.z = 0.f;
    Rota.z += 4 * DT;
    Transform()->SetRelativeRotation(Rota);

    // 공통 이펙트 업데이트
    static float Time = 0.f;
    Time += 3 * DT;
    if (Time >= 0.3f)
        Time = 0.f;
    GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, Time);

    m_CurrentTime += DT;

    // 타입별 동작 처리
    switch (m_Type)
    {
        case 1: // 웨이포인트 순회
        {
            if (m_CurrentTime >= m_LiveTime)
            {
                DestroyObject(GetOwner());
                return;
            }

            Vec3 currentPos = Transform()->GetRelativePos();
            Vec3 targetPos = m_Waypoints[m_CurrentWaypoint];
            Vec3 dir = targetPos - currentPos;

            if (dir.Length() < 10.f)  // 웨이포인트 도달
            {
                m_CurrentWaypoint = (m_CurrentWaypoint + 1) % m_Waypoints.size();
            }
            else
            {
                dir.Normalize();
                Transform()->SetRelativePos(currentPos + dir * 300.f * DT);
            }
        }
        break;

        case 2: // 오른쪽으로 이동하며 플레이어 Y값 추적
        {
            CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CGameObject* Player = CurLevel->FindObjectByName(L"Player");

            if (Player)
            {
                Vec3 currentPos = GetOwner()->Transform()->GetRelativePos();
                float playerY = Player->Transform()->GetRelativePos().y;

                // 중간 객체(인덱스 1)를 기준으로 위아래 300씩 배치
                float targetY = playerY + (m_GroupIndex - 1) * 300.f;
                float t = 2.f * DT;

                // Y값 보간
                currentPos.y = currentPos.y + (targetY - currentPos.y) * t;

                // X값은 지속적으로 증가
                currentPos.x += 300.f * DT;

                Transform()->SetRelativePos(currentPos);

                if (currentPos.x > 17000.f)
                {
                    DestroyObject(GetOwner());
                }
            }
        }
        break;

        case 3: // 위아래로 조여오는 동작
        {
            float startDelay = m_GroupIndex * 0.5f;  // 그룹별 시작 딜레이
            if (m_CurrentTime < startDelay)
                return;

            Vec3 currentPos = Transform()->GetRelativePos();
            bool isUpperGroup = currentPos.y > 0.f;

            // 목표 Y 위치 (중앙에서 약간 떨어진 지점)
            float targetY = isUpperGroup ? 40.f : -150.f;

            // 현재 Y 위치가 목표 지점에 도달하지 않았다면 천천히 이동
            if ((isUpperGroup && currentPos.y > targetY) ||
                (!isUpperGroup && currentPos.y < targetY))
            {
                if (isUpperGroup)
                    currentPos.y -= 45.f * DT;  // 속도 감소
                else
                    currentPos.y += 45.f * DT;  // 속도 감소

                Transform()->SetRelativePos(currentPos);
            }
        }
        break;

        case 4:
        {
            if (m_CurrentTime >= m_LiveTime)
            {
                DestroyObject(GetOwner());
                return;
            }

            Vec3 currentPos = Transform()->GetRelativePos();
            Vec3 targetPos = m_bForward ? m_PatternEndPos : m_PatternStartPos;
            Vec3 moveDir = (targetPos - currentPos);
            float distance = moveDir.Length();

            if (distance < 5.f)  // 목표 지점 도달
            {
                m_bForward = !m_bForward;  // 방향 전환
            }
            else
            {
                moveDir.Normalize();
                Vec3 newPos = currentPos + moveDir * m_MoveSpeed * DT;
                Transform()->SetRelativePos(newPos);
            }


        }
        break;
    }

}

void CSircleObj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSircleObj::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSircleObj::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSircleObj::SaveComponent(FILE* _File)
{
}

void CSircleObj::LoadComponent(FILE* _File)
{
}



