#pragma once
#include "CScript.h"

class TurretScript :
    public CScript
{
private:
    float       AccTime;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);


public:
    CLONE(TurretScript);
    TurretScript();
    ~TurretScript();
};
