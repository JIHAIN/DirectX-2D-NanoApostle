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

    // 모든 창 간격 동일화 하기
    ImGui::PushItemWidth(200.f);

	CParticleSystem* pParticle = GetTargetObject()->ParticleSystem();
    int* MaxParticCount = pParticle->GetMaxParticleCount();
    tParticleModule* ParticleModule = pParticle->GetParticleModule();

    // 텍스쳐 설정할 수 있게 하기
    ImGui::Text("Texture");
    ImGui::SameLine(120);
    ImGui::SetNextItemWidth(200);
    m_Texture = pParticle->GetParticleTexture();
    m_TextureKey = string(m_Texture->GetRelativePath().begin(), m_Texture->GetRelativePath().end());

    // 파일 이름만 추출
    size_t lastSlash = m_TextureKey.find_last_of("//\\\\");  // '/' 또는 '\' 의 마지막 위치 찾기
    string fileName = (lastSlash != string::npos) ? m_TextureKey.substr(lastSlash + 1) : m_TextureKey;

    static char buffer[256];
    strcpy_s(buffer, fileName.c_str());
    ImGui::InputText("##KeySet", buffer, sizeof(buffer));  // m_TextureKey 대신 fileName 사용

    ImGui::SameLine(330);

    if (ImGui::Button("Load##11", ImVec2(60.f, 18.f)))
    {
        m_TextureKey = SetParticleTexture(); // 파일탐색기로 텍스쳐 불러올 수 있게함
    }

    ////////////////////////////////////
    ImGui::Text("MaxParticle");
    ImGui::SameLine(120);

    ImGui::InputInt("##MAXCOUNT", MaxParticCount, 10);
 
    if (ImGui::Button("Particle Save##22", ImVec2(120.f, 18.f)))
    {
        SaveParticle(m_Texture); //만든파일 저장
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

        // .x 붙이는 이유 시작주소를 줘야함
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
            // Module 배열의 크기만큼 반복하며 모든 값을 0으로 설정
            for (int i = 0; i < 7; i++)  // MODULE_SIZE는 배열의 크기
            {
                _Module->Module[i] = 0;
            }
        }
        Clear = false;
    }
}


//HLSL 보면서 만든 필요 조작 인자들
//1. 당연히 스폰 카운트 조절
//2. SpawnShapeType 박스인지 구인지, 그냥 BlockSpawnShape 타입도 똑같이 바꿔버리자
//3. AddVelocityModule 켜있는지 켜있다면 AddVelocityType이 뭔지
//0 = Random, 1 = FromCenter, 2 = ToCenter 3 ~그외 Fixed 고정인듯
//
//여기 까지가 이제 스폰 될떄 해야하는 일 즉 그니까 인풋락에서 내가 1빠로 들어왔을때
//세팅 받는 값들
//
//다음 내가 활성화 되면 해야할일
//1. NoiseForce 할지 말지
//2. SpaceType 정해주기 Local 과 Wordl 가 있음
//3. ScaleModule 설정하기
//4. DragModule 인데 쌤이 엘스쪽 조건 없앴네 왠지 봐야할듯
//5. Render 설정

void ParticleUI::DrawLabeledInput(const char* label, const std::function<void()>& inputWidget)
{
    ImGui::Text(label);
    ImGui::SameLine(120);  // 일관된 간격
    inputWidget();
}



string ParticleUI::SetParticleTexture()
{
    wstring StrContentPath = CPathMgr::GetInst()->GetContentPath();
    //StrContentPath += L"texture\\";

    wchar_t szFilePath[255] = {};

    OPENFILENAME Desc = {}; // 파일 구조체 초기화
    Desc.lStructSize = sizeof(OPENFILENAME); //사이즈
    Desc.hwndOwner = nullptr; // 별 안중요함
    Desc.lpstrFile = szFilePath; // 아까 초기화한 배열 넣어줌
    Desc.nMaxFile = 255; //우리가 목적지로 설정한 배열의 최대 갯수
    Desc.lpstrFilter = L"PNG\0*.png\0ALL\0*.*";
    Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 이건 고정
    Desc.lpstrInitialDir = StrContentPath.c_str();

    if (GetSaveFileName(&Desc))
    {
        if (wcslen(szFilePath) != 0)
        {
            // 총 파일 길이
            wstring fullPath = szFilePath;

            // 찾고자 하는 경로를 제외한 나머지를 제거 strContentPath 에는 텍스쳐\\ 까지의 경로가 들어있음
            size_t pos = fullPath.find(StrContentPath);

            if (pos != wstring::npos)
            {
                // 'StrContentPath' 이후의 경로만 남기기
                fullPath = fullPath.substr(pos + StrContentPath.length());

                // 먼저 텍스쳐 불러오고
                m_Texture = CAssetMgr::GetInst()->Load<CTexture>(fullPath, fullPath);

                // 파티클 텍스쳐 수정해주기
                GetTargetObject()->ParticleSystem()->SetParticleTexture(m_Texture);

                // 받아낸 이름 반환해주기
                return string(fullPath.begin(), fullPath.end());
            }

        }
    }
    // 함수의 반환 타입이 string 이므로 모든 조건에 걸리지 않을 경우 기본값인 빈 문자열 반환


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

    // 파티클 이름
    fwprintf(pFile, L"[PARTICLE_NAME]\n");
    fwprintf(pFile, L"%s\n\n", strName.c_str());

    // 텍스처 정보
    fwprintf(pFile, L"[ATLAS_TEXTURE]\n");
    if (_Texture != nullptr)
    {
        fwprintf(pFile, L"%s\n", _Texture->GetKey().c_str());
        fwprintf(pFile, L"%s\n\n", _Texture->GetRelativePath().c_str());
    }

    // 모듈 활성화 정보
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