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
	m_LiveTime = 25.f;  // �⺻ �����ð�
	m_StartPos = Transform()->GetRelativePos();
	m_CurrentWaypoint = 0;

	if (m_Type == 1)
	{
		// ��������Ʈ ����
		m_Waypoints.push_back(Vec3(1812.f, 154.f, 1.f));
		m_Waypoints.push_back(Vec3(2338.f, 154.f, 1.f));
		m_Waypoints.push_back(Vec3(2338.f, -416.f, 1.f));
		m_Waypoints.push_back(Vec3(1812.f, -416.f, 1.f));
	}
    else if (m_Type == 4)
    {
        Vec3 currentPos = Transform()->GetRelativePos();
        if (abs(currentPos.x - 388.f) < 1.f)  // ù ��° ��ü
        {
            SetPatternPoints(Vec3(388.f, 1500.f, 1.f), Vec3(388.f, 1750.f, 1.f));
        }
        else if (abs(currentPos.x - 1050.f) < 1.f)  // �� ��° ��ü
        {
            SetPatternPoints(Vec3(1050.f, 1500.f, 1.f), Vec3(1790.f, 1500.f, 1.f));
        }
        else  // �� ��° ��ü
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
    m_MoveSpeed = 300.f;  // �̵� �ӵ� ����
}

void CSircleObj::Tick()
{
    // ���� ȸ�� ȿ��
    Vec3 Rota = Transform()->GetRelativeRotation();
    if (Rota.z >= XM_PI * 2)
        Rota.z = 0.f;
    Rota.z += 4 * DT;
    Transform()->SetRelativeRotation(Rota);

    // ���� ����Ʈ ������Ʈ
    static float Time = 0.f;
    Time += 3 * DT;
    if (Time >= 0.3f)
        Time = 0.f;
    GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, Time);

    m_CurrentTime += DT;

    // Ÿ�Ժ� ���� ó��
    switch (m_Type)
    {
        case 1: // ��������Ʈ ��ȸ
        {
            if (m_CurrentTime >= m_LiveTime)
            {
                DestroyObject(GetOwner());
                return;
            }

            Vec3 currentPos = Transform()->GetRelativePos();
            Vec3 targetPos = m_Waypoints[m_CurrentWaypoint];
            Vec3 dir = targetPos - currentPos;

            if (dir.Length() < 10.f)  // ��������Ʈ ����
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

        case 2: // ���������� �̵��ϸ� �÷��̾� Y�� ����
        {
            CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CGameObject* Player = CurLevel->FindObjectByName(L"Player");

            if (Player)
            {
                Vec3 currentPos = GetOwner()->Transform()->GetRelativePos();
                float playerY = Player->Transform()->GetRelativePos().y;

                // �߰� ��ü(�ε��� 1)�� �������� ���Ʒ� 300�� ��ġ
                float targetY = playerY + (m_GroupIndex - 1) * 300.f;
                float t = 2.f * DT;

                // Y�� ����
                currentPos.y = currentPos.y + (targetY - currentPos.y) * t;

                // X���� ���������� ����
                currentPos.x += 300.f * DT;

                Transform()->SetRelativePos(currentPos);

                if (currentPos.x > 17000.f)
                {
                    DestroyObject(GetOwner());
                }
            }
        }
        break;

        case 3: // ���Ʒ��� �������� ����
        {
            float startDelay = m_GroupIndex * 0.5f;  // �׷캰 ���� ������
            if (m_CurrentTime < startDelay)
                return;

            Vec3 currentPos = Transform()->GetRelativePos();
            bool isUpperGroup = currentPos.y > 0.f;

            // ��ǥ Y ��ġ (�߾ӿ��� �ణ ������ ����)
            float targetY = isUpperGroup ? 40.f : -150.f;

            // ���� Y ��ġ�� ��ǥ ������ �������� �ʾҴٸ� õõ�� �̵�
            if ((isUpperGroup && currentPos.y > targetY) ||
                (!isUpperGroup && currentPos.y < targetY))
            {
                if (isUpperGroup)
                    currentPos.y -= 45.f * DT;  // �ӵ� ����
                else
                    currentPos.y += 45.f * DT;  // �ӵ� ����

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

            if (distance < 5.f)  // ��ǥ ���� ����
            {
                m_bForward = !m_bForward;  // ���� ��ȯ
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



