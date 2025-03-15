#pragma once
#include <Engine/CScript.h>

class CPlayer_Arrow_Obj :
    public CScript
{
private:
    Vec3    m_Velocity;
    float   m_Speed;
    float   m_DeleteTime;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;


public:
    CLONE(CPlayer_Arrow_Obj);

    CPlayer_Arrow_Obj(Vec3 _Velocity);
    CPlayer_Arrow_Obj();
    ~CPlayer_Arrow_Obj();
};