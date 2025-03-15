#pragma once
#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{
public:
    virtual void Render_Update() override;
    void RenderCameraLayerCheckUI(CCamera* pCamera);
    void HoveredCamerInfo(int _i);

public:
    CameraUI();
    ~CameraUI();
};


