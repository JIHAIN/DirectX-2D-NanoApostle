#pragma once
#include "EditorUI.h"


class MenuUI :
    public EditorUI
{
private:

public:
    virtual void Render() override;
    virtual void Render_Update() override {};

private:
    void File();
    void Level();
    void GameObject();
    void Editor();
    void Asset();
    void Animation();
    void CInspector();

    void LoadLevelFile(const wstring& _levelName);

    wstring GetNextMaterialName();

public:
    MenuUI();
    ~MenuUI();
};
