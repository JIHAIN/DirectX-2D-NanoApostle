#pragma once
#include <Engine/CScript.h>

class CPlayer_Main :
    public CScript
{
private:
    float m_Time;

    CGameObject* pParticle;
    CGameObject* pParticle1;
    CGameObject* PostObj;
    void createPost();

public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void AI_In() override;
    virtual void AI_Tick() override;
    virtual void AI_Exit() override;

    tParticleModule ParticleSetting();
    tParticleModule ParticleSetting1();


public:
    CLONE(CPlayer_Main);
    CPlayer_Main();
    ~CPlayer_Main();
};

