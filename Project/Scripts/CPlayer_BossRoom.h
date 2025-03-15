#pragma once
#include <Engine/CScript.h>

class CPlayer_BossRoom :
    public CScript
{
private:
    float m_Time;
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

public:
    CLONE(CPlayer_BossRoom);
    CPlayer_BossRoom();
    ~CPlayer_BossRoom();
};

