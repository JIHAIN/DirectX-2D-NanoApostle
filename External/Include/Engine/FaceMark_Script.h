#pragma once
#include "CScript.h"

class FaceMark_Script :
    public CScript
{

public:
    virtual void Begin() override;
    virtual void Tick() override;

    float GetAngle8Dir(const Vec3& dir, float _playerRotationY);

public:
    CLONE_DISABLE(FaceMark_Script);

    FaceMark_Script();
    ~FaceMark_Script();
};

