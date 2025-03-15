#pragma once
#include <Engine/CScript.h>

class CGameObject;
class CPlayer_Ai;

class CFaceMark_Script :
    public CScript
{
private:
    CGameObject* m_Player;
    CPlayer_Ai* m_PlayerScript;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    float GetAngle8Dir(const Vec3& dir, float _playerRotationY);


public:
    CLONE(CFaceMark_Script);

    CFaceMark_Script();
    ~CFaceMark_Script();
};

