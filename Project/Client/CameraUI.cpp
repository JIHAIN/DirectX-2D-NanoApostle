#include "pch.h"
#include "CameraUI.h"

#include <Engine/CCamera.h>
#include <Engine/CRenderMgr.h>

CameraUI::CameraUI()
    : ComponentUI("CameraUI", COMPONENT_TYPE::CAMERA)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_Update()
{
    ComponentTitle("Camera");

    CCamera* pCamera = GetTargetObject()->Camera();

    ImGui::Text("Projection Type");
    ImGui::SameLine(120);

    // PROJ_TYPE
    const char* szProjType[2] = { "Perspective", "Orthographic" };
    int ProjType = pCamera->GetProjType();
    ImGui::Combo("##combo", &ProjType, szProjType, 2);
    pCamera->SetProjType((PROJ_TYPE)ProjType);

    // Far
    ImGui::Text("Far");
    ImGui::SameLine(120);
    float Far = pCamera->GetFar();
    ImGui::InputFloat("##Far", &Far);

    // Far 가 Near(1) 보다 작거나 같으면 안된다. 시야범위는 Near 에서 Far 사이이기 때문
    if (Far <= 1.f)
        Far = 1.1f;
    pCamera->SetFar(Far);

    // Width
    float Width = pCamera->GetWidth();
    float AspectRatio = pCamera->GetAspectRatio();
    float FOV = pCamera->GetFOV();

    ImGui::Text("Width");
    ImGui::SameLine(120);
    ImGui::InputFloat("##Width", &Width);
    pCamera->SetWidth(Width);

    ImGui::Text("AspectRatio");
    ImGui::SameLine(120);
    ImGui::DragFloat("##AspectRatio", &AspectRatio, 0.01f);
    pCamera->SetAspectRatio(AspectRatio);

    // 현재 카메라 투영 옵션이 직교투영이면 비활성화
    ImGui::BeginDisabled(ProjType == (int)PROJ_TYPE::ORTHOGRAPHIC);

    // 원근투영 전용
    ImGui::Text("Field Of View");
    ImGui::SameLine(120);
    FOV = (FOV * 180.f) / XM_PI;
    ImGui::DragFloat("##FOV", &FOV, 0.1f);
    FOV = (FOV * XM_PI) / 180.f;
    pCamera->SetFOV(FOV);

    ImGui::EndDisabled();


    // 현재 카메라 투영 옵션이 직교투영이면 비활성화
    ImGui::BeginDisabled(ProjType == (int)PROJ_TYPE::PERSPECTIVE);

    // 직교투영 전용 변수
    float Scale = pCamera->GetScale();

    ImGui::Text("Scale");
    ImGui::SameLine(120);
    FOV = (FOV * 180.f) / XM_PI;
    ImGui::DragFloat("##Scale", &Scale, 0.1f);
    FOV = (FOV * XM_PI) / 180.f;

    // 직교투영 배율이 0 이하로 내려가면 안된다.
    if (Scale <= 0.f)
        Scale = 0.1f;

    pCamera->SetScale(Scale);

    ImGui::EndDisabled();

    //===========================
    ImGui::Text("Priority");
    ImGui::SameLine(120);

    vector<CCamera*> VecCam = CRenderMgr::GetInst()->GetVecCamer();

    int Priority = pCamera->GetCameraPriority();
    if (ImGui::InputInt("##Priority", &Priority))
    {
        Priority = min(Priority, 20);
        Priority = max(Priority, -1);

        CRenderMgr::GetInst()->ChangeCamera(pCamera, Priority);
    }

    RenderCameraLayerCheckUI(pCamera);

}

void CameraUI::RenderCameraLayerCheckUI(CCamera* pCamera)
{
    ImGui::Text("Layer Check");
    ImGui::SameLine(120);

    UINT layerCheck = pCamera->GetLayerCheck();
    bool changed = false;

    for (int i = 0; i < 32; ++i)  // Assuming 32 layers maximum
    {
        if (i > 0 && i % 8 == 0)
            ImGui::NewLine();
        else if (i > 0)
            ImGui::SameLine();

        if (i % 8 == 0)
        {
            ImGui::SameLine(120);
        }

        bool isChecked = (layerCheck & (1 << i)) != 0;
        if (ImGui::Checkbox(("##Layer" + std::to_string(i)).c_str(), &isChecked))
        {
            changed = true;
            if (isChecked)
                layerCheck |= (1 << i);
            else
                layerCheck &= ~(1 << i);
        }

        HoveredCamerInfo(i);
    }

    if (changed)
    {
        pCamera->SetLayerCheck(layerCheck);
    }
}

void CameraUI::HoveredCamerInfo(int _i)
{
    if (ImGui::IsItemHovered())
    {
        switch (_i)
        {
        case 0:
            return ImGui::SetTooltip("Background");
        case 1:
            return ImGui::SetTooltip("Tile");
        case 2:
            return ImGui::SetTooltip("Default");
        case 3:
            return ImGui::SetTooltip("Player");
        case 4:
            return ImGui::SetTooltip("PlayerObject");
        case 5:
            return ImGui::SetTooltip("Monster");
        case 6:
            return ImGui::SetTooltip("MonsterObject");
        default:
            return ImGui::SetTooltip("Layer %d", _i);
        }

    }
}
