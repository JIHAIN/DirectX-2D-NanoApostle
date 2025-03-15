#pragma once
#include <Engine/CScript.h>

class CMap_BossMain :
    public CScript
{
private:
    CGameObject* pParticle1;
    CGameObject* pParticle2;
    CGameObject* PostObj;
    void createPost();

    float m_Time;
    bool  m_WarpOn;
public:
    virtual void Tick() override;
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    tParticleModule ParticleSetting1();
    tParticleModule ParticleSetting2();

public:
    CLONE(CMap_BossMain);
    CMap_BossMain();
    ~CMap_BossMain();
};

