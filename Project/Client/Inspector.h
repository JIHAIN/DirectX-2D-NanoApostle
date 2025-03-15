#pragma once
#include "EditorUI.h"


#include <Engine/Ptr.h>
#include <Engine/assets.h>

class CGameObject;
class ComponentUI;
class ScriptUI;

class AssetUI;

class Inspector :
    public EditorUI
{
private:
    CGameObject*        m_TargetObject;
    ComponentUI*        m_arrComUI[(UINT)COMPONENT_TYPE::END];

    Ptr<CAsset>         m_TargetAsset;
    AssetUI*            m_arrAssetUI[(UINT)ASSET_TYPE::END];

    vector<ScriptUI*>   m_vecScriptUI;

public:
    void SetTargetObject(CGameObject* _Target);
    void SetTargetAsset(Ptr<CAsset> _Asset);

public:
    virtual void Render_Update() override;
    virtual void Deactivate() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();

public:
    Inspector();
    ~Inspector();
};

