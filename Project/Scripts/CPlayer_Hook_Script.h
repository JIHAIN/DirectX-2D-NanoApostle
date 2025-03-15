#pragma once
#include <Engine/CScript.h>

class CPlayer_Hook_Script :
    public CScript
{
private:
    int     AtkDmg;
    Vec3    m_MyPos;         // 시작 위치
    CGameObject*    m_Monster;   // 목표
    CGameObject*    m_Chain;    // 체인
    float   m_fDashDuration;    // 대시 지속시간
    float   m_fCurrentTime;     // 현재 진행된 시간

public:
    virtual void Begin() override;
    virtual void Tick() {};
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    void PlayerAniPlay1(Vec3 _Dir);

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE(CPlayer_Hook_Script);
    CPlayer_Hook_Script();
    ~CPlayer_Hook_Script();
};

