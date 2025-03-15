#pragma once
#include <Engine/CScript.h>

class Player_Arrow_Script :
    public CScript
{
private:

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    void CreateArrow();

public:
    CLONE_DISABLE(Player_Arrow_Script);

    Player_Arrow_Script();
    ~Player_Arrow_Script();
};

