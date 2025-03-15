#pragma once
#include <Engine/CScript.h>

class CShield_Script :
    public CScript
{
private:
    CGameObject* pParticle;
    CGameObject* m_Shield;
    Vec3         MousDir;
    float        m_CreateShield_Time;
    bool    m_Parrying;
    float   m_ParryingTime;

    void CreateShield();

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    tParticleModule ParticleSetting();
    void PlayerAniPlay(Vec3 _Dir);
    bool IsParry() { return m_Parrying; }

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:

public:
    CLONE(CShield_Script);
    CShield_Script();
    ~CShield_Script();
};

