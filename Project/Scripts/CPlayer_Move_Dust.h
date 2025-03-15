#pragma once
#include <Engine/CScript.h>

class CPlayer_Move_Dust :
    public CScript
{
private:

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CPlayer_Move_Dust);

    CPlayer_Move_Dust();
    ~CPlayer_Move_Dust();
};
