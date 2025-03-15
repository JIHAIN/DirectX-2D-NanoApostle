#pragma once
#include <Engine/CScript.h>

class CBoss_Jump_landing :
    public CScript
{
private:
    CGameObject* pParticle;

public:
    tParticleModule ParticleSetting();

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
public:
    CLONE(CBoss_Jump_landing);
    CBoss_Jump_landing();
    ~CBoss_Jump_landing();
};

