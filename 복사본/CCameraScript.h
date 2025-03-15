#pragma once
#include <Engine/CScript.h>

class CCameraScript :
    public CScript
{
private:
    float   m_CameraSpeed;

public:
    virtual void Tick() override;

private:
    void CameraOrthgraphicMove();
    void CameraPerspectiveMove();

public:
    CCameraScript();
    ~CCameraScript();
};

