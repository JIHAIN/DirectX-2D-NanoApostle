#pragma once
#include <Engine/CScript.h>

class CBoss_Roar :
    public CScript
{
private:
    CGameObject* pPostProcess;
    bool m_first;
    bool m_AniStart;
    float m_Time;
public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE(CBoss_Roar);
    CBoss_Roar();
    ~CBoss_Roar();
};

