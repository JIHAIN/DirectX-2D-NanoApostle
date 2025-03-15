#pragma once
#include <Engine/CScript.h>

class CBoss_Dead :
    public CScript
{
    CGameObject*    m_Excute_Obj;
    bool            m_Destroy;
public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void AI_In() override;
    virtual void AI_Tick() override;
    virtual void AI_Exit() override;

    void CreateAtk();
public:
    CLONE(CBoss_Dead);
    CBoss_Dead();
    ~CBoss_Dead();
};

