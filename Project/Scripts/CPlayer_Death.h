#pragma once
#include <Engine/CScript.h>

class CPlayer_Death :
    public CScript
{
    CGameObject* m_DeathPost;
public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void AI_In() override;
    virtual void AI_Tick() override;
    virtual void AI_Exit() override;

    void CreatePost();
public:
    CLONE(CPlayer_Death);
    CPlayer_Death();
    ~CPlayer_Death();
};

