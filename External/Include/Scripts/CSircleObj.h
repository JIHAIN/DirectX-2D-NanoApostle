#pragma once
#include <Engine/CScript.h>

class CSircleObj :
    public CScript
{
private:
    int m_Type;
    float m_LiveTime;     // 생존 시간
    float m_CurrentTime;  // 현재 시간
    Vec3 m_StartPos;      // 시작 위치
    int m_GroupIndex;     // 타입3에서 사용할 그룹 인덱스

    // 패턴 4용
    Vec3 m_PatternStartPos;  // 패턴 시작 위치
    Vec3 m_PatternEndPos;    // 패턴 끝 위치
    bool m_bForward;         // 이동 방향 (true: 시작->끝, false: 끝->시작)
    float m_MoveSpeed;       // 이동 속도

    // 타입1용 웨이포인트
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