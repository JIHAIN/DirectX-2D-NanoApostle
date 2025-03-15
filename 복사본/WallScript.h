#pragma once
#include <Engine/CScript.h>

class WallScript :
    public CScript
{
private:
    CGameObject* m_Player;

    Vec2 PrevPos;
    Vec2 vOthObjPos;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);


public:
    CLONE(WallScript);
    WallScript();
    ~WallScript();
};


