#include "pch.h"
#include "MenuUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include "CLevelSaveLoad.h"

#include "CImGuiMgr.h"
#include "Inspector.h"

#include "MainLevel.h"
#include "BossLevel.h"
#include "TrapLevel.h"
#include "ALevel.h"

MenuUI::MenuUI()
    : EditorUI("MenuUI")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Render()
{
    if (!IsActive())
        return;

    if (ImGui::BeginMainMenuBar())
    {
        File();

        Level();

        GameObject();

        CInspector();

        Animation();

        Editor();

        Asset();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Level Save"))
        {
            wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
            //strFilePath += L"Level\\Test.lv";

            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            wstring levelName = pCurLevel->GetName();
            wstring levelPath = L"Level\\" + levelName + L".lv";

            strFilePath += levelPath;

            CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevelSaveLoad::SaveLevel(strFilePath, CurLevel);
        }

        //if (ImGui::MenuItem("Level Load"))
        //{
        //    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
        //    strFilePath += L"Level\\Test.lv";

        //    CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(strFilePath);
        //    ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

        //    // 레벨이 로드될때 Inspector 에서 보여주던 정보를 전부 제거한다. (삭제된 객체를 가리키고 있을 수 있기 때문)
        //    Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
        //    pInspector->SetTargetObject(nullptr);
        //}
        if (ImGui::BeginMenu("Level Load"))
        {
            if (ImGui::MenuItem("Main"))
            {
                MainLevel::CreateMainLevel();
                Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
                pInspector->SetTargetObject(nullptr);
            }
            if (ImGui::MenuItem("Trap"))
            {
                TrapLevel::CreateTrapLevel();
                Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
                pInspector->SetTargetObject(nullptr);
            }
            if (ImGui::MenuItem("Boss"))
            {
                BossLevel::CreateBossLevel();
                Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
                pInspector->SetTargetObject(nullptr);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void MenuUI::LoadLevelFile(const wstring& _levelName)
{
    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += L"Level\\" + _levelName + L".lv";
    CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(strFilePath);
    ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

    Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
    pInspector->SetTargetObject(nullptr);
}

void MenuUI::Level()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    LEVEL_STATE State = LEVEL_STATE::NONE;

    if (nullptr != pCurLevel)
        State = pCurLevel->GetState();

    bool IsNotPlay = State != LEVEL_STATE::PLAY;
    bool IsPlay = !IsNotPlay;
    bool IsPause = State == LEVEL_STATE::PAUSE;

    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("Play", nullptr, nullptr, IsNotPlay))
        {
            ChangeLevelState(LEVEL_STATE::PLAY);

        /*    wstring levelName = pCurLevel->GetName();
            wstring levelPath = L"Level\\" + levelName + L".lv";
            CLevelSaveLoad::SaveLevel(CPathMgr::GetInst()->GetContentPath() + levelPath, pCurLevel);*/

            CLevelSaveLoad::SaveLevel(CPathMgr::GetInst()->GetContentPath() + L"Level\\Temp.lv", pCurLevel);

            ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, IsPlay))
        {
            ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, IsPlay || IsPause))
        {
            // 레벨이 정지될때 Inspector 에서 보여주던 정보를 전부 제거한다. (삭제된 객체를 가리키고 있을 수 있기 때문)
            class Inspector* pInspector = (class Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            pInspector->SetTargetObject(nullptr);

            CLevel* pLevel = CLevelSaveLoad::LoadLevel(CPathMgr::GetInst()->GetContentPath() + L"Level\\Temp.lv");
            ChangeLevel(pLevel, LEVEL_STATE::STOP);
        }

        ImGui::EndMenu();
    }

}

void MenuUI::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
}

void MenuUI::Editor()
{
    if (ImGui::BeginMenu("Editor"))
    {

        ImGui::EndMenu();
    }
}

void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material", nullptr, nullptr))
        {
            Ptr<CMaterial> pMtrl = new CMaterial;
            wstring MtrlKey = GetNextMaterialName();
            CAssetMgr::GetInst()->AddAsset(MtrlKey, pMtrl);
        }

        ImGui::EndMenu();
    }
}

wstring MenuUI::GetNextMaterialName()
{
    wchar_t Bufff[255] = {};

    UINT i = 0;
    while (true)
    {
        wsprintf(Bufff, L"Material\\Default Material %d.mtrl", i++);

        if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(Bufff))
            break;
    }

    return Bufff;
}


void MenuUI::Animation()
{
   EditorUI* Ani = CImGuiMgr::GetInst()->FindUI("AnimationUI");

    if (ImGui::BeginMenu("Animation"))
    {
        //wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
        //strFilePath += L"Level\\A.lv";

       // CLevel* TestLevel = CLevelSaveLoad::LoadLevel(strFilePath);
       // ChangeLevel(TestLevel, LEVEL_STATE::PLAY);
        ALevel::CreateALevel();
        CLevelMgr::GetInst()->RequestLevelLoad(L"A");

        Ani->SetActive(true);

        ImGui::EndMenu();
    }
}

void MenuUI::CInspector()
{
    EditorUI* Inspector = CImGuiMgr::GetInst()->FindUI("Inspector");
    EditorUI* Outliner = CImGuiMgr::GetInst()->FindUI("Outliner");
    EditorUI* Content = CImGuiMgr::GetInst()->FindUI("Content");

    if (ImGui::BeginMenu("Inspector##12"))
    {
        Inspector->SetActive(true);
        Outliner->SetActive(true);
        Content->SetActive(true);

        ImGui::EndMenu();
    }
}
