#pragma once
#include <Engine/CScript.h>

class Shield_Script :
    public CScript
{
private:
    CGameObject* pParticle;
    CGameObject* pParticle2;

    CGameObject* pParticle3;

    CGameObject* pParticle4;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    void CreateArrow();

public:
    CLONE_DISABLE(Shield_Script);

    Shield_Script();
    ~Shield_Script();
};

