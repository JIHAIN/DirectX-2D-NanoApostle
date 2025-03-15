#pragma once
#include <Engine/CScript.h>

class CPlayer_End :
    public CScript
{
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
    CLONE(CPlayer_End);
    CPlayer_End();
    ~CPlayer_End();
};