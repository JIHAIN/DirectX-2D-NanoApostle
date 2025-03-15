#pragma once
#include <Engine/CScript.h>

class CBoss_Dash :
    public CScript
{
private:
    float        m_fDashDuration;    // 대시 지속시간
    float        m_fCurrentTime;     // 현재 진행된 시간
    Vec3         m_vStartPos;         // 시작 위치
    Vec3         m_vTargetPos;        // 목표 위치

    CGameObject* m_Dash_Obj;
public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void AI_In() override;
    virtual void AI_Tick() override;
    virtual void AI_Exit() override;

    void CreateAtk();

public:
    CLONE(CBoss_Dash);
    CBoss_Dash();
    ~CBoss_Dash();
};

