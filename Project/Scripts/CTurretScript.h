#pragma once
#include <Engine/CScript.h>

class CTurretScript :
    public CScript
{
private:
    float        AccTime;
    CGameObject* m_TargetPlayer;
    bool         m_Groggy;
    bool         m_First;

    CGameObject* pParticle;
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override {};
    virtual void LoadComponent(FILE* _File) override {};

    tParticleModule ParticleSetting();

public:
    CLONE(CTurretScript);
    CTurretScript();
    ~CTurretScript();
};
