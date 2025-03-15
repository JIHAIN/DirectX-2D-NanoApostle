#pragma once
#include <Engine/CScript.h>

class CEB :
    public CScript
{
private:
    CGameObject* m_Player;
    bool         m_Goal;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CEB);
    CEB();
    ~CEB();
};

