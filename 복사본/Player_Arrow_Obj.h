#pragma once
#include <Engine/CScript.h>

class Player_Arrow_Obj :
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

public:
    CLONE_DISABLE(Player_Arrow_Obj);

    Player_Arrow_Obj(Vec3 _Velocity);
    ~Player_Arrow_Obj();
};