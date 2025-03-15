#pragma once
#include <Engine/CScript.h>

class CShield_Script_Obj :
    public CScript
{
private:
    bool    m_Parrying;
    float   m_ParryingTime;

    CGameObject* pParticle;
    CGameObject* pParticle1;
    CGameObject* pParticle2;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    bool IsParry() { return m_Parrying; }

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    tParticleModule ParticleSetting();
    tParticleModule ParticleSetting1();
    tParticleModule ParticleSetting2();
public:
    CLONE(CShield_Script_Obj);
    CShield_Script_Obj();
    ~CShield_Script_Obj();
};