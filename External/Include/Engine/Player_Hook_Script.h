#pragma once
#include "CScript.h"
class Player_Hook_Script :
    public CScript
{
private:
    int     AtkDmg;
    float   AtkRange;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE_DISABLE(Player_Hook_Script);
    Player_Hook_Script();
    ~Player_Hook_Script();
};

