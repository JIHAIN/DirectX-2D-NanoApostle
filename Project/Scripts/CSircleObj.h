#pragma once
#include <Engine/CScript.h>

class CSircleObj :
    public CScript
{
private:
    int m_Type;
    float m_LiveTime;     // ���� �ð�
    float m_CurrentTime;  // ���� �ð�
    Vec3 m_StartPos;      // ���� ��ġ
    int m_GroupIndex;     // Ÿ��3���� ����� �׷� �ε���

    // ���� 4��
    Vec3 m_PatternStartPos;  // ���� ���� ��ġ
    Vec3 m_PatternEndPos;    // ���� �� ��ġ
    bool m_bForward;         // �̵� ���� (true: ����->��, false: ��->����)
    float m_MoveSpeed;       // �̵� �ӵ�

    // Ÿ��1�� ��������Ʈ
    vector<Vec3> m_Waypoints;
    int m_CurrentWaypoint;

    void SetPatternPoints(Vec3 _start, Vec3 _end);

public:
    virtual void Tick() override;
    virtual void Begin() override;

    void SetType(int _Type) { m_Type = _Type; }
    void SetGroupIndex(int _index) { m_GroupIndex = _index; }


    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CSircleObj);
    CSircleObj();
    ~CSircleObj();
};