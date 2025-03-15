#pragma once
#include "ComponentUI.h"
class ScriptUI :
    public ComponentUI
{
private:
    CScript* m_TargetScript;

public:
    void SetScript(CScript* _Script);

public:
    virtual void Render_Update() override;

public:
    ScriptUI();
    ~ScriptUI();
};

