#include "pch.h"
#include "ParticleUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

ParticleUI::ParticleUI()
	: ComponentUI("ParticleUI", COMPONENT_TYPE::PARTICLE_SYSTEM)
{

}

ParticleUI::~ParticleUI()
{
}

void ParticleUI::Render_Update()
{
	ComponentTitle("Particle");

    // ��� â ���� ����ȭ �ϱ�
    ImGui::PushItemWidth(200.f);

	CParticleSystem* pParticle = GetTargetObject()->ParticleSystem();
    int* MaxParticCount = pParticle->GetMaxParticleCount();
    tParticleModule* ParticleModule = pParticle->GetParticleModule();

    // �ؽ��� ������ �� �ְ� �ϱ�
    ImGui::Text("Texture");
    ImGui::SameLine(120);
    ImGui::SetNextItemWidth(200);
    m_Texture = pParticle->GetParticleTexture();
    m_TextureKey = string(m_Texture->GetRelativePath().begin(), m_Texture->GetRelativePath().end());

    // ���� �̸��� ����
    size_t lastSlash = m_TextureKey.find_last_of("//\\\\");  // '/' �Ǵ� '\' �� ������ ��ġ ã��
    string fileName = (lastSlash != string::npos) ? m_TextureKey.substr(lastSlash + 1) : m_TextureKey;

    static char buffer[256];
    strcpy_s(buffer, fileName.c_str());
    ImGui::InputText("##KeySet", buffer, sizeof(buffer));  // m_TextureKey ��� fileName ���

    ImGui::SameLine(330);

    if (ImGui::Button("Load##11", ImVec2(60.f, 18.f)))
    {
        m_TextureKey = SetParticleTexture(); // ����Ž����� �ؽ��� �ҷ��� �� �ְ���
    }

    ////////////////////////////////////
    ImGui::Text("MaxParticle");
    ImGui::SameLine(120);

    ImGui::InputInt("##MAXCOUNT", MaxParticCount, 10);
 
    if (ImGui::Button("Particle Save##22", ImVec2(120.f, 18.f)))
    {
        SaveParticle(m_Texture); //�������� ����
    }

    ImGui::NewLine();

    ComponentTitle("Module Type");
    
    CheakBoxList(ParticleModule);

    ImGui::NewLine();

    if(ParticleModule->Module[0])
    { 
        ComponentTitle("SPAWN");
     
        ImGui::Text("SpawnRate");
        ImGui::SameLine(150);
        ImGui::InputScalar("##SpawnRate", ImGuiDataType_U32, &(ParticleModule->SpawnRate));
      
        ImGui::Text("SpawnColor");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##vSpawnColor", &(ParticleModule->vSpawnColor.x) , 0.1f);
    
        ImGui::Text("MinLife");
        ImGui::SameLine(150);
        ImGui::DragFloat("##MinLife", &(ParticleModule->MinLife), 0.1f);

        ImGui::Text("MaxLife");
        ImGui::SameLine(150);
        ImGui::DragFloat("##MaxLife", &(ParticleModule->MaxLife), 0.1f);

        // .x ���̴� ���� �����ּҸ� �����
        ImGui::Text("SpawnMinScale");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##SpawnMinScale", &(ParticleModule->vSpawnMinScale.x) , 0.1f);
     
        ImGui::Text("SpawnMaxScale");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##SpawnMaxScale", &(ParticleModule->vSpawnMaxScale.x), 0.1f);
        
        ImGui::Text("SpawnShape");
        ImGui::SameLine(150);
        const char* szSShape[2] = { "Box", "Sphere" };
        ImGui::Combo("##SpawnShape", (int*)&(ParticleModule->SpawnShape), szSShape, 2);

        ImGui::Text("SpawnShapeScale");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##SpawnShapeScale", &(ParticleModule->SpawnShapeScale.x), 1.f);

        ImGui::Text("BlockSpawnShape");
        ImGui::SameLine(150);       
        const char* szBSShape[2] = { "Box", "Sphere" };
        ImGui::Combo("##BlockSpawnShape", (int*)&(ParticleModule->BlockSpawnShape), szBSShape, 2);

        ImGui::Text("BlockSpawnShapeScale");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##BlockSpawnShapeScale", &(ParticleModule->BlockSpawnShapeScale.x));

        ImGui::Text("SpaceType"); 
        ImGui::SameLine(150);
        const char* szSPType[2] = { "Local", "World" };
        ImGui::Combo("##SpaceType", (int*)&(ParticleModule->SpaceType), szSPType, 2);

        ImGui::NewLine();
    }


    if (ParticleModule->Module[1])
    {
        ComponentTitle("SPAWN_BURST");
 
        ParticleModule->SpawnBurstRepeat = true;

        ImGui::Text("SpawnBurstCount");
        ImGui::SameLine(150);      
        ImGui::InputScalar("##SpawnBurstCount", ImGuiDataType_U32, &(ParticleModule->SpawnBurstCount));

        ImGui::Text("SpawnBurstRepeatTime");
        ImGui::SameLine(150);
        ImGui::DragFloat("##SpawnBurstRepeatTime", &(ParticleModule->SpawnBurstRepeatTime), 0.1f);

        ImGui::NewLine();

    }

    if (ParticleModule->Module[2])
    {
        ComponentTitle("ADD_VELOCITY");

        ImGui::Text("AddVelocityType");
        ImGui::SameLine(150);
        const char* szVelocityType[4] = { "Random", "FromCenter" , "ToCenter", "Fixed" };
        ImGui::Combo("##AddVelocityType", (int*)&(ParticleModule->AddVelocityType), szVelocityType, IM_ARRAYSIZE(szVelocityType));

        ImGui::Text("AddVelocityFixedDir");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##AddVelocityFixedDir", &(ParticleModule->AddVelocityFixedDir.x), 1.f);

        ImGui::Text("AddMinSpeed");
        ImGui::SameLine(150);
        ImGui::DragFloat("##AddMinSpeed", &(ParticleModule->AddMinSpeed), 0.1f);

        ImGui::Text("AddMaxSpeed");
        ImGui::SameLine(150);
        ImGui::DragFloat("##AddMaxSpeed", &(ParticleModule->AddMaxSpeed), 0.1f);

        ImGui::NewLine();
    }

    if (ParticleModule->Module[3])
    {
        ComponentTitle("SCALE");

        ImGui::Text("StartScale");
        ImGui::SameLine(150);
        ImGui::DragFloat("##StartScale", &(ParticleModule->StartScale), 0.1f);

        ImGui::Text("EndScale");
        ImGui::SameLine(150);
        ImGui::DragFloat("##EndScale", &(ParticleModule->EndScale), 0.1f);

        ImGui::NewLine();

    }

    if (ParticleModule->Module[4])
    {
        ComponentTitle("DRAG");

        ImGui::Text("DestNormalizedAge");
        ImGui::SameLine(150);
        ImGui::DragFloat("##DestNormalizedAge", &(ParticleModule->DestNormalizedAge), 1.f);

        ImGui::Text("LimitSpeed");
        ImGui::SameLine(150);
        ImGui::DragFloat("##LimitSpeed", &(ParticleModule->LimitSpeed), 1.f);

        ImGui::NewLine();

    }

    if (ParticleModule->Module[5])
    {
        ComponentTitle("NOISE_FORCE");

        ImGui::Text("NoiseForceTerm");
        ImGui::SameLine(150);
        ImGui::DragFloat("##NoiseForceTerm", &(ParticleModule->NoiseForceTerm), 0.1f);

        ImGui::Text("NoiseForceScale");
        ImGui::SameLine(150);
        ImGui::DragFloat("##NoiseForceScale", &(ParticleModule->NoiseForceScale), 1.f);

        ImGui::NewLine();

    }

    if (ParticleModule->Module[6])
    {
        ComponentTitle("RENDER");

        ImGui::Text("EndColor");
        ImGui::SameLine(150);
        ImGui::DragFloat3("##EndColor", &(ParticleModule->EndColor.x), 0.1f);

        ImGui::Text("FadeOut");
        ImGui::SameLine(150);
        const char* szNoiseType[2] = { "Off", "Normalized Age" };
        ImGui::Combo("##FadeOut", (int*)&(ParticleModule->FadeOut), szNoiseType, 2);

        ImGui::Text("FadeOutStartRatio ");
        ImGui::SameLine(150);
        ImGui::DragFloat("##FadeOutStartRatio ", &(ParticleModule->FadeOutStartRatio), 0.01f);

        ImGui::Text("VelocityAlignment");
        ImGui::SameLine(150);
        const char* szVelocType[2] = { "Off", "On" };
        ImGui::Combo("##VelocityAlignment", (int*)&(ParticleModule->VelocityAlignment), szVelocType, 2);

        ImGui::NewLine();

    }


}

void ParticleUI::CheakBoxList(tParticleModule* _Module)
{
    ImGui::Text("SPAWN");
    ImGui::SameLine();
    bool SPAWN = (_Module->Module[0] != 0);
    if (ImGui::Checkbox("##Check", &SPAWN))
    {
        _Module->Module[0] = SPAWN ? 1 : 0;
    }

    ImGui::SameLine(80);
    ImGui::Text("SPAWN_BURST");
    ImGui::SameLine();
    bool SPAWN_BURST = (_Module->Module[1] != 0);
    if (ImGui::Checkbox("##Check1", &SPAWN_BURST))
    {
        _Module->Module[1] = SPAWN_BURST ? 1 : 0;
    }

    ImGui::SameLine(280);
    ImGui::Text("ADD_VELOCITY");
    ImGui::SameLine();
    bool ADD_VELOCITY = (_Module->Module[2] != 0);
    if (ImGui::Checkbox("##Check2", &ADD_VELOCITY))
    {
        _Module->Module[2] = ADD_VELOCITY ? 1 : 0;
    }

    ImGui::SameLine(200);
    ImGui::Text("SCALE");
    ImGui::SameLine(250);
    bool SCALE = (_Module->Module[3] != 0);
    if (ImGui::Checkbox("##Check3", &SCALE))
    {
        _Module->Module[3] = SCALE ? 1 : 0;
    }

    ImGui::Text("DRAG");
    ImGui::SameLine(43);
    bool DRAG = (_Module->Module[4] != 0);
    if (ImGui::Checkbox("##Check4", &DRAG))
    {
        _Module->Module[4] = DRAG ? 1 : 0;
    }
    ImGui::SameLine(80);
    ImGui::Text("NOISE_FORCE");
    ImGui::SameLine();
    bool NOISE_FORCE = (_Module->Module[5] != 0);
    if (ImGui::Checkbox("##Check5", &NOISE_FORCE))
    {
        _Module->Module[5] = NOISE_FORCE ? 1 : 0;
    }

    ImGui::SameLine(200);
    ImGui::Text("RENDER");
    ImGui::SameLine();
    bool RENDER = (_Module->Module[6] != 0);
    if (ImGui::Checkbox("##Check6", &RENDER))
    {
        _Module->Module[6] = RENDER ? 1 : 0;
    }

    ImGui::SameLine(280);
    ImGui::Text("All Clear");
    ImGui::SameLine(372);
    bool Clear = false;
    if (ImGui::Checkbox("##Check7", &Clear))
    {
        if (Clear)
        {
            // Module �迭�� ũ�⸸ŭ �ݺ��ϸ� ��� ���� 0���� ����
            for (int i = 0; i < 7; i++)  // MODULE_SIZE�� �迭�� ũ��
            {
                _Module->Module[i] = 0;
            }
        }
        Clear = false;
    }
}


//HLSL ���鼭 ���� �ʿ� ���� ���ڵ�
//1. �翬�� ���� ī��Ʈ ����
//2. SpawnShapeType �ڽ����� ������, �׳� BlockSpawnShape Ÿ�Ե� �Ȱ��� �ٲ������
//3. AddVelocityModule ���ִ��� ���ִٸ� AddVelocityType�� ����
//0 = Random, 1 = FromCenter, 2 = ToCenter 3 ~�׿� Fixed �����ε�
//
//���� ������ ���� ���� �ɋ� �ؾ��ϴ� �� �� �״ϱ� ��ǲ������ ���� 1���� ��������
//���� �޴� ����
//
//���� ���� Ȱ��ȭ �Ǹ� �ؾ�����
//1. NoiseForce ���� ����
//2. SpaceType �����ֱ� Local �� Wordl �� ����
//3. ScaleModule �����ϱ�
//4. DragModule �ε� ���� ������ ���� ���ݳ� ���� �����ҵ�
//5. Render ����

void ParticleUI::DrawLabeledInput(const char* label, const std::function<void()>& inputWidget)
{
    ImGui::Text(label);
    ImGui::SameLine(120);  // �ϰ��� ����
    inputWidget();
}



string ParticleUI::SetParticleTexture()
{
    wstring StrContentPath = CPathMgr::GetInst()->GetContentPath();
    //StrContentPath += L"texture\\";

    wchar_t szFilePath[255] = {};

    OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
    Desc.lStructSize = sizeof(OPENFILENAME); //������
    Desc.hwndOwner = nullptr; // �� ���߿���
    Desc.lpstrFile = szFilePath; // �Ʊ� �ʱ�ȭ�� �迭 �־���
    Desc.nMaxFile = 255; //�츮�� �������� ������ �迭�� �ִ� ����
    Desc.lpstrFilter = L"PNG\0*.png\0ALL\0*.*";
    Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �̰� ����
    Desc.lpstrInitialDir = StrContentPath.c_str();

    if (GetSaveFileName(&Desc))
    {
        if (wcslen(szFilePath) != 0)
        {
            // �� ���� ����
            wstring fullPath = szFilePath;

            // ã���� �ϴ� ��θ� ������ �������� ���� strContentPath ���� �ؽ���\\ ������ ��ΰ� �������
            size_t pos = fullPath.find(StrContentPath);

            if (pos != wstring::npos)
            {
                // 'StrContentPath' ������ ��θ� �����
                fullPath = fullPath.substr(pos + StrContentPath.length());

                // ���� �ؽ��� �ҷ�����
                m_Texture = CAssetMgr::GetInst()->Load<CTexture>(fullPath, fullPath);

                // ��ƼŬ �ؽ��� �������ֱ�
                GetTargetObject()->ParticleSystem()->SetParticleTexture(m_Texture);

                // �޾Ƴ� �̸� ��ȯ���ֱ�
                return string(fullPath.begin(), fullPath.end());
            }

        }
    }
    // �Լ��� ��ȯ Ÿ���� string �̹Ƿ� ��� ���ǿ� �ɸ��� ���� ��� �⺻���� �� ���ڿ� ��ȯ


    return "";
}

int ParticleUI::SaveParticle(Ptr<CTexture> _Texture)
{
    wstring Path = CPathMgr::GetInst()->GetContentPath();
    wstring filePath = L"Particle\\";
    Path += filePath;

    wstring strName = m_Texture->GetKey();
    wstring FilePath = Path + strName;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, FilePath.c_str(), L"w");

    if (!pFile)
    {
        MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
        return FALSE;
    }

    tParticleModule* ParticleModule = GetTargetObject()->ParticleSystem()->GetParticleModule();

    // ��ƼŬ �̸�
    fwprintf(pFile, L"[PARTICLE_NAME]\n");
    fwprintf(pFile, L"%s\n\n", strName.c_str());

    // �ؽ�ó ����
    fwprintf(pFile, L"[ATLAS_TEXTURE]\n");
    if (_Texture != nullptr)
    {
        fwprintf(pFile, L"%s\n", _Texture->GetKey().c_str());
        fwprintf(pFile, L"%s\n\n", _Texture->GetRelativePath().c_str());
    }

    // ��� Ȱ��ȭ ����
    fwprintf(pFile, L"[MODULE_STATES]\n");
    for (int i = 0; i < 7; ++i)
    {
        fwprintf(pFile, L"Module_%d %d\n", i, ParticleModule->Module[i]);
    }
    fwprintf(pFile, L"\n");

    // Spawn Module
    fwprintf(pFile, L"[SPAWN_MODULE] %s\n", ParticleModule->Module[0] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"SpawnRate %u\n", ParticleModule->SpawnRate);
    fwprintf(pFile, L"SpawnColor %.3f %.3f %.3f %.3f\n",
        ParticleModule->vSpawnColor.x, ParticleModule->vSpawnColor.y,
        ParticleModule->vSpawnColor.z, ParticleModule->vSpawnColor.w);
    fwprintf(pFile, L"SpawnMinScale %.3f %.3f %.3f\n",
        ParticleModule->vSpawnMinScale.x, ParticleModule->vSpawnMinScale.y, ParticleModule->vSpawnMinScale.z);
    fwprintf(pFile, L"SpawnMaxScale %.3f %.3f %.3f\n",
        ParticleModule->vSpawnMaxScale.x, ParticleModule->vSpawnMaxScale.y, ParticleModule->vSpawnMaxScale.z);
    fwprintf(pFile, L"LifeRange %.3f %.3f\n", ParticleModule->MinLife, ParticleModule->MaxLife);
    fwprintf(pFile, L"SpawnShape %u\n", ParticleModule->SpawnShape);
    fwprintf(pFile, L"SpawnShapeScale %.3f %.3f %.3f\n",
        ParticleModule->SpawnShapeScale.x, ParticleModule->SpawnShapeScale.y, ParticleModule->SpawnShapeScale.z);
    fwprintf(pFile, L"BlockSpawnShape %u\n", ParticleModule->BlockSpawnShape);
    fwprintf(pFile, L"BlockSpawnShapeScale %.3f %.3f %.3f\n",
        ParticleModule->BlockSpawnShapeScale.x, ParticleModule->BlockSpawnShapeScale.y, ParticleModule->BlockSpawnShapeScale.z);
    fwprintf(pFile, L"SpaceType %u\n\n", ParticleModule->SpaceType);

    // Spawn Burst Module
    fwprintf(pFile, L"[SPAWN_BURST_MODULE] %s\n", ParticleModule->Module[1] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"SpawnBurstCount %u\n", ParticleModule->SpawnBurstCount);
    fwprintf(pFile, L"SpawnBurstRepeat %u\n", ParticleModule->SpawnBurstRepeat);
    fwprintf(pFile, L"SpawnBurstRepeatTime %.3f\n\n", ParticleModule->SpawnBurstRepeatTime);

    // Add Velocity Module
    fwprintf(pFile, L"[ADD_VELOCITY_MODULE] %s\n", ParticleModule->Module[2] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"AddVelocityType %u\n", ParticleModule->AddVelocityType);
    fwprintf(pFile, L"AddVelocityFixedDir %.3f %.3f %.3f\n",
        ParticleModule->AddVelocityFixedDir.x, ParticleModule->AddVelocityFixedDir.y, ParticleModule->AddVelocityFixedDir.z);
    fwprintf(pFile, L"SpeedRange %.3f %.3f\n\n", ParticleModule->AddMinSpeed, ParticleModule->AddMaxSpeed);

    // Scale Module
    fwprintf(pFile, L"[SCALE_MODULE] %s\n", ParticleModule->Module[3] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"ScaleRange %.3f %.3f\n\n", ParticleModule->StartScale, ParticleModule->EndScale);

    // Drag Module
    fwprintf(pFile, L"[DRAG_MODULE] %s\n", ParticleModule->Module[4] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"DestNormalizedAge %.3f\n", ParticleModule->DestNormalizedAge);
    fwprintf(pFile, L"LimitSpeed %.3f\n\n", ParticleModule->LimitSpeed);

    // Noise Force Module
    fwprintf(pFile, L"[NOISE_FORCE_MODULE] %s\n", ParticleModule->Module[5] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"NoiseForceTerm %.3f\n", ParticleModule->NoiseForceTerm);
    fwprintf(pFile, L"NoiseForceScale %.3f\n\n", ParticleModule->NoiseForceScale);

    // Render Module
    fwprintf(pFile, L"[RENDER_MODULE] %s\n", ParticleModule->Module[6] ? L"TRUE" : L"FALSE");
    fwprintf(pFile, L"EndColor %.3f %.3f %.3f\n",
        ParticleModule->EndColor.x, ParticleModule->EndColor.y, ParticleModule->EndColor.z);
    fwprintf(pFile, L"FadeOut %u\n", ParticleModule->FadeOut);
    fwprintf(pFile, L"FadeOutStartRatio  %.3f\n", ParticleModule->FadeOutStartRatio);
    fwprintf(pFile, L"VelocityAlignment %u\n", ParticleModule->VelocityAlignment);

    fclose(pFile);
    return S_OK;
}