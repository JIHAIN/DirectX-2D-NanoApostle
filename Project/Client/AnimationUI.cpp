#include "pch.h"
#include "AnimationUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/components.h>
#include <Engine/CTexture.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CScript.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CCamera.h>
#include <Engine/CGameObject.h>

#include <windows.h>
#include <commdlg.h>


AnimationUI::AnimationUI()
	: EditorUI("AnimationUI")
	, m_Flipbook(new CFlipbook)
	, m_Texture(nullptr)
	, m_AniInfo{}
	, m_AtlasInfo{}
	, m_PreviewObj(nullptr)
	, m_LeftTop(0.f, 0.f)
	, m_Slice(0.f, 0.f)
	, m_BackGround(200.f, 200.f)
	, m_Offset(0.f, 0.f)
	, m_SpriteIdx(0)
	, m_FPS(3.f)
	, m_Time(0.f)
	, m_Finish(false)
	, CreatingMode(true)
	, m_TextureKey("")
	, m_TexturePath("")
	, m_FlibookKey("")
	, m_FlibookKeyBuffer(256)
	, m_Idex(0)
	, m_PrevIdex(0)
	, m_SpriteKey("")
	, m_spriteKeyBuffer(256)
	, m_WheelScale(1.f)
	, m_IsPanning(false)
	, m_Ratio(1.f)
	, m_WidthSize(0.f)
{
}

AnimationUI::~AnimationUI()
{
}

void AnimationUI::Render_Update()
{
	const char* szLayerType[2] = { "Create", "Edit" };
	static int LayerType = 0;

	ComponentTitle("Mode");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.0f);
	// 레이어 선택 콤보 박스
	// 테스크 매니저 처럼 자기 인자들 다 초기화 시키고 다음틱에 변경되게 하자

	if (ImGui::Combo("##combo2", &LayerType, szLayerType, 2))
	{
		if (LayerType == 0)
		{
			CreatingMode = true;
			m_PreviewObj = nullptr;
			Clear();
			m_Flipbook = new CFlipbook;
		}
		else
		{
			CreatingMode = false;

			if (m_PreviewObj != nullptr)
				DestroyObject(m_PreviewObj);

			Clear();
			m_Flipbook = new CFlipbook;
		}
			
	}

	ImGui::NewLine();

	if (CreatingMode)
	{
		CreateMode();
		
	}
	else
		EditMode();

	if (CreatingMode)
	{
		ImGui::Begin("##Tes1");
		RenderAtlasPreview();
		ImGui::End();
	}
	else
	{
		ImGui::Begin("##Tes2");
		RenderAnimationPreview();

		ImGui::End();
	}

	// 스프라이트 프리뷰 창
	//RenderSpritePreview();

	// 애니메이션 프리뷰 창
	//RenderAnimationPreview();

	// 구현은 완료 개선점 플립북을 저장하고 나면 new 로 새로운 플립북을 만들어줘야함 지금은 생성자에서 
	// new 할당을 하기 때문에 하나의 플립북 밖에 만들지 못함. 
	// 그리고 텍스쳐 로딩 하면 이제 배경화면에 텍스쳐 띄어야함 그리고 내가 지정한걸 볼 수 있게 해줘야함 
	// 조정할 수 있게 그리고 수정 및 이미 만들걸 다시 볼 수 있게도 만들어야함.
	// 저장이 됐으면 됐다고 표시 되면 좋을 거 같음.
}


void AnimationUI::ComponentTitle(const string& _title)
{
	ImGui::PushID(0);

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));

	ImGui::Button(_title.c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void AnimationUI::Clear()
{
	if (m_PreviewObj != nullptr)
	{
		DestroyObject(m_PreviewObj);
		m_PreviewObj = nullptr;
	}

	// 내 멤버 다 초기화하기
	m_Flipbook = nullptr;
	m_Texture = nullptr;
	m_AniInfo = {};
	m_AtlasInfo = {};
	m_CurSprite.clear();

	m_TextureKey = " ";
	m_TexturePath = " ";

	m_FlibookKey = " ";
	m_FlibookKeyBuffer.assign(m_FlibookKeyBuffer.size(), '\0');

	m_Idex = 0; //스프라이틑 인덱스 저장용 키값의 이름에도 이걸 넣음
	m_PrevIdex = 0;
	m_SpriteKey = " ";
	m_spriteKeyBuffer.assign(m_spriteKeyBuffer.size(), '\0');
	    
	m_LeftTop = Vec2(0.f, 0.f);
	m_Slice = Vec2(0.f, 0.f);
	m_BackGround = Vec2(200.f, 200.f);
	m_Offset = Vec2(0.f, 0.f);

	m_SpriteIdx = 0;
	m_FPS = 5.f;
	m_Time = 0.f;
	m_Finish = false;

	m_WheelScale = 1.f;
}

Vec2 AnimationUI::UVToPixel(int textureWidth, int textureHeight, const Vec2& uv)
{
	return Vec2(uv.x * textureWidth, uv.y * textureHeight);
}

Vec2 AnimationUI::PixelToUV(int textureWidth, int textureHeight, const Vec2& pixel)
{
	return Vec2(pixel.x / textureWidth, pixel.y / textureHeight);
}

string AnimationUI::SelectAtlas()
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
			wstring selectedFile = szFilePath;

			// 총 파일 길이
			wstring fileName = selectedFile.substr(selectedFile.find_last_of(L"\\") + 1);

			wstring relativePath = L"Texture\\" + fileName;
			m_Texture = CAssetMgr::GetInst()->Load<CTexture>(relativePath, relativePath);
			return string(relativePath.begin(), relativePath.end());
		}
	}
	// 함수의 반환 타입이 string 이므로 모든 조건에 걸리지 않을 경우 기본값인 빈 문자열 반환


	return "";
}


int AnimationUI::SaveFlipbook(Ptr<CTexture> _Texture)
{
	wstring Path = CPathMgr::GetInst()->GetContentPath();
	wstring strName = L" ";
	strName = m_Flipbook->GetName();
	

	// 전체 경로를 한번에 구성
	wstring FilePath = Path + L"Animation\\" + strName + L".anim";

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, FilePath.c_str(), L"w");

	if (!pFile)
	{
		// 파일 열기에 실패한 경우
		MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
		return FALSE; // 함수 종료
	}

	// 애니메이션 이름
	fwprintf(pFile, L"[ANIMATION_NAME]\n");
	fwprintf(pFile, L"%s\n\n", strName.c_str());

	// 참조하던 아틀라스 텍스쳐 정보
	fwprintf(pFile, L"[ATLAS_TEXTURE]\n");
	if (_Texture != nullptr)
	{
		fwprintf(pFile, L"%s\n", _Texture->GetKey().c_str());
		fwprintf(pFile, L"%s\n", _Texture->GetRelativePath().c_str());
	}
	else
	{
		fwprintf(pFile, L"None\n");
		fwprintf(pFile, L"None\n");
	}
	fwprintf(pFile, L"\n");
	
	// 내가 보유한 플립북이 보유한 벡터 스프라이트 받아와서 거기에 저장된 내용 만큼 파일로 저장함.
	vector<Ptr<CSprite>> m_vecSprite = m_Flipbook->GetVecSprite();

	// 각 프레임별 데이터
	fwprintf(pFile, L"[FRAME_DATA]\n");
	fwprintf(pFile, L"Frame_Count %d\n\n", static_cast<int>(m_vecSprite.size()));

	// 텍스쳐 크기 정보
	int TWidth = _Texture.Get()->GetWidth();
	int THeight = _Texture.Get()->GetHeight();

	for (size_t i = 0; i < m_vecSprite.size(); ++i)
	{
		// UV 좌표를 픽셀 좌표로 변환해서 저장시킴
		Vec2 LeftTop = UVToPixel(TWidth, THeight, m_vecSprite[i]->GetLeftTop());
		Vec2 Slice = UVToPixel(TWidth, THeight, m_vecSprite[i]->GetSlice());
		Vec2 BackGround = UVToPixel(TWidth, THeight, m_vecSprite[i]->GetBackground());
		Vec2 Offset = UVToPixel(TWidth, THeight, m_vecSprite[i]->GetOffset());

		fwprintf(pFile, L"Sprite_Index %d\n", static_cast<int>(i));
		fwprintf(pFile, L"Key          %s\n", m_vecSprite[i]->GetKey().c_str());
		fwprintf(pFile, L"Left_Top    %.1f %.1f\n", LeftTop.x, LeftTop.y);
		fwprintf(pFile, L"Offset      %.1f %.1f\n", Offset.x, Offset.y);
		fwprintf(pFile, L"Slice       %.1f %.1f\n", Slice.x, Slice.y);
		fwprintf(pFile, L"BackGround  %.1f %.1f\n", BackGround.x, BackGround.y);
		fwprintf(pFile, L"\n");
	}

	fclose(pFile);
	return S_OK;
}

string AnimationUI::LoadFlipbook()
{
	// 파일 탐색기 설정
	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // 부모 윈도우 핸들이 있다면 여기에 지정
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All Files (*.*)\0*.*\0Animation Files (*.anim)\0*.anim\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	wstring Path = CPathMgr::GetInst()->GetContentPath();
	Path += L"Animation";


	// 초기 디렉토리 설정
	wchar_t initialDir[MAX_PATH];
	wcscpy_s(initialDir, Path.c_str());
	
	ofn.lpstrInitialDir = initialDir;

	 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		// 파일 열기
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, szFile, L"r");

		if (!pFile)
		{
			// 파일 열기에 실패한 경우
			MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
			return " "; // 함수 종료
		}

		wchar_t szRead[255] = {};
		wstring AniName;
		Ptr<CTexture> EditAtlas = nullptr; 	// 설정할 텍스쳐 미리 선언
		m_Flipbook = new CFlipbook;

		while (true)
		{
			if (EOF == fwscanf_s(pFile, L"%s", szRead, 255))
				break;

			wstring str = szRead;

			if (str == L"[ANIMATION_NAME]")
			{
				fwscanf_s(pFile, L"%s", szRead, 255);

				AniName = szRead;
				m_Flipbook->SetName(AniName);
			}
			else if (str == L"[ATLAS_TEXTURE]")
			{
				wstring strKey, strRelativePath;
				fwscanf_s(pFile, L"%s", szRead, 255);
				strKey = szRead;

				fwscanf_s(pFile, L"%s", szRead, 255);
				strRelativePath = szRead;

				if (strKey != L"None")
				{
					// 텍스쳐 찾아서 받아둠
					EditAtlas = CAssetMgr::GetInst()->Load<CTexture>(strKey, strRelativePath);
				}
			}
			else if (str == L"[FRAME_DATA]")
			{
				wchar_t FrameBuff[255] = {};

				int frameCount = 0;
				// 개행 문자 처리
				fgetwc(pFile);
				
				fwscanf_s(pFile, L"Frame_Count %d", &frameCount);

				//SpriteInfo 를 카운트 수만큼 준비함
				vector<SpriteInfo> VecSpInfo;
				SpriteInfo SInfo = { };

				//키값 다 받아둠 
				wchar_t szkeyRead[255] = {};
				vector<wstring> vecspriteKey;

				while (true)
				{

					if (EOF == fwscanf_s(pFile, L"%s", FrameBuff, 255))
					{
						break;
					}
					
					if (!wcscmp(L"Key", FrameBuff))
					{
						fwscanf_s(pFile, L"%s", szkeyRead, 255);
						vecspriteKey.push_back(szkeyRead);
					}
					else if (!wcscmp(L"Left_Top", FrameBuff))
					{
						fwscanf_s(pFile, L"%f %f", &SInfo.LeftTopUV.x, &SInfo.LeftTopUV.y);
					}
					else if (!wcscmp(L"Offset", FrameBuff))
					{
						fwscanf_s(pFile, L"%f %f", &SInfo.OffsetUV.x, &SInfo.OffsetUV.y);
					}
					else if (!wcscmp(L"Slice", FrameBuff))
					{
						fwscanf_s(pFile, L"%f %f", &SInfo.SliceUV.x, &SInfo.SliceUV.y);
					}
					else if (!wcscmp(L"BackGround", FrameBuff))
					{
						fwscanf_s(pFile, L"%f %f", &SInfo.BackgroundUV.x, &SInfo.BackgroundUV.y);
						VecSpInfo.push_back(SInfo);
					}
				}

				for (int i = 0; i < frameCount; ++i) // 찾은 프레임 수대로 
				{
					Ptr<CSprite> pSprite = new CSprite;
					pSprite->SetAtlasTexture(EditAtlas);
					pSprite->SetLeftTop(VecSpInfo[i].LeftTopUV);
					pSprite->SetOffset(VecSpInfo[i].OffsetUV);
					pSprite->SetSlice(VecSpInfo[i].SliceUV);
					pSprite->SetBackground(VecSpInfo[i].BackgroundUV);

					CAssetMgr::GetInst()->AddAsset(vecspriteKey[i], pSprite);
					m_Flipbook->AddSprite(pSprite);
				}
			}
		}

		// flipbook 등록
		CAssetMgr::GetInst()->AddAsset(m_Flipbook->GetName(), m_Flipbook);

		fclose(pFile);

		return string(m_Flipbook->GetName().begin(), m_Flipbook->GetName().end());
	}
	else
	{
		// 파일 선택을 취소한 경우
		return " ";
	}

}

void AnimationUI::CreateMode()
{
	ComponentTitle("Texture");

	m_TextureKey.resize(256);
	m_TexturePath.resize(256);

	ImGui::Text("Texture");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(220);
	ImGui::InputText("##KeySet", m_TextureKey.data(), m_TextureKey.size());

	Vec2 kResolution = CDevice::GetInst()->GetRenderResolution();
	static Vec2 DebugRectScale = Vec2(0.f, 0.f);

	ImGui::NewLine();
	ImGui::SameLine(100);

	if (m_Flipbook == nullptr)
		m_Flipbook = new CFlipbook;

	if (ImGui::Button("Load##11", ImVec2(60.f, 18.f)))
	{
		m_TextureKey = SelectAtlas(); // 파일탐색기로 텍스쳐 불러올 수 있게함

		if (m_Texture != nullptr)
		{
			float TextureX = (float)m_Texture.Get()->GetWidth();
			float TextureY = (float)m_Texture.Get()->GetHeight();

			DebugRectScale = Vec2(TextureX, TextureY); // 디버그 그리기용 위치 넘겨주기

			wstring AtlasKey = wstring(m_TextureKey.begin(), m_TextureKey.end());
			m_AtlasInfo.AtlasTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(AtlasKey);

			m_WheelScale = 1.f;
		}
	}

	if (m_Texture != nullptr)
	{
		DrawDebugRect(Vec4(0.f, 1.f, 0.f, 0.5f), Vec3((0 - kResolution.x / 2) + (DebugRectScale.x / 2), (0 + kResolution.y / 2) - DebugRectScale.y / 2, 1.f)
			, Vec2(DebugRectScale.x, DebugRectScale.y), Vec3(0.f, 0.f, 0.f), false, 0.f);
	}
	else
		DebugRectScale = Vec2(0.f, 0.f); // 텍스쳐 없을시에는 초기화 

	ComponentTitle("Sprite Set");

	// 하나씩 해보자 
	// 마우스 위치 받아오는거 부터 시작
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePosCam();

	ImGui::Text("LeftTop");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("##LeftTopX", &m_LeftTop.x);

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("##LeftTopY", &m_LeftTop.y);

	//m_LeftTop.x = trunc(m_LeftTop.x);
	//m_LeftTop.y = trunc(m_LeftTop.y);

	ImGui::Text("Slice");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("##SliceX", &m_Slice.x);

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("##SliceY", &m_Slice.y);

	//m_Slice.x = trunc(m_Slice.x);
	//m_Slice.y = trunc(m_Slice.y);

	ImGui::Text("BackGround");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	ImGui::InputFloat("##BackX", &m_BackGround.x, 10);
	m_BackGround.y = m_BackGround.x;

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	ImGui::InputFloat("##BackY", &m_BackGround.y, 10);
	m_BackGround.x = m_BackGround.y;

	ImGui::Text("OffSet");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	ImGui::InputFloat("##OffX", &m_Offset.x, 1);

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	ImGui::InputFloat("##OffY", &m_Offset.y, 1);

	// 여기까지가 마우스를 이용해서 좌상단 , 슬라이스 값 구하기 및 디버그로 표시하기 백그라운드 설정 오프셋 설정

	Ptr<CSprite> pSprite = nullptr;  // 스프라이트 준비 

	ImGui::Text("Sprite Name");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(135);

	if (ImGui::InputText("##KeySet123", m_spriteKeyBuffer.data(), m_spriteKeyBuffer.size()))
	{
		m_SpriteKey = string(m_spriteKeyBuffer.data());
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(77);
	ImGui::InputInt("##Index", &m_Idex, 1);

	ImGui::NewLine();
	ImGui::SameLine(100);

	if (KEY_TAP(KEY::ENTER) || ImGui::Button("ADD##123", ImVec2(40.f, 18.f))) // 버튼을 누르면 이제 위에서 설정한것들을 넣어줌
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(m_Texture);
		pSprite->SetLeftTop(m_LeftTop);
		pSprite->SetSlice(m_Slice);
		pSprite->SetBackground(m_BackGround);
		pSprite->SetOffset(m_Offset);

		++m_Idex;

		wstring szKey = {}; // 키값 저장용
		szKey = wstring(m_SpriteKey.begin(), m_SpriteKey.end());

		szKey += L"_" + std::to_wstring(m_Idex); // 뒤에 언더바와 인덱스 번호 넣기

		CAssetMgr::GetInst()->AddAsset(szKey, pSprite);

		// 스프라이트 에셋 등록하고 그 프레임 창에 띄우기
		m_AniInfo.Texture = CAssetMgr::GetInst()->FindAsset<CSprite>(szKey)->GetAtlasTexture();

		m_AniInfo.LeftTop = pSprite->GetLeftTop();
		m_AniInfo.Slice = pSprite->GetSlice();
		m_AniInfo.BackGround = pSprite->GetBackground();
		m_AniInfo.Offset = pSprite->GetOffset();

		m_Flipbook->AddSprite(pSprite);
	}

	ComponentTitle("Flibook Set");

	ImGui::Text("Flibook Name");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(135);

	if (ImGui::InputText("##Set333", m_FlibookKeyBuffer.data(), m_FlibookKeyBuffer.size()))
	{
		// string으로 변환 (널문자까지만)
		m_FlibookKey = string(m_FlibookKeyBuffer.data());

		// wstring으로 변환
		m_Flipbook->SetName(wstring(m_FlibookKey.begin(), m_FlibookKey.end()));
	}

	ImGui::SameLine();
	if (ImGui::Button("SAVE##323", ImVec2(40.f, 18.f)))
	{
		CAssetMgr::GetInst()->AddAsset(wstring(m_FlibookKey.begin(), m_FlibookKey.end()), m_Flipbook);
		m_Idex = 0;
		//////////////////
		vector<Ptr<CSprite>> SpriteOffsetting = m_Flipbook->GetVecSprite();

		// 첫 번째 프레임의 발 위치를 기준으로 계산
		Ptr<CSprite> baseSprite = SpriteOffsetting[0];
		float baseSliceHeight = baseSprite->GetPixelSlice().y;
		float baseBackgroundHeight = baseSprite->GetPixelBackground().y;

		// 기준 프레임에서 슬라이스가 백그라운드 내에서 위치하는 중앙 Y위치
		float baseCenter = (baseBackgroundHeight - baseSliceHeight) * 0.5f;

		for (size_t i = 1; i < SpriteOffsetting.size(); ++i) 
		{

			Ptr<CSprite> sprite = SpriteOffsetting[i];
			float sliceHeight = sprite->GetPixelSlice().y;
			float backgroundHeight = sprite->GetPixelBackground().y;

			// 현재 프레임의 중앙 Y위치
			float currentCenter = (backgroundHeight - sliceHeight) * 0.5f;

			// 기준 프레임과의 차이를 오프셋으로 설정
			float offset = baseCenter - currentCenter;

			sprite->SetOffset(Vec2(0.f, -offset));
		}

		////////////////
		SaveFlipbook(m_Texture);

		m_Flipbook = nullptr;
	}

	// 스프라이트 프리뷰 창
	RenderSpritePreview();
}

void AnimationUI::EditMode()
{
	// 해야할 거 백그라운드 적용시켜서 내가 수정하는게 올바르게 보이게 해야함
	// 지금은 조정해도 티가 안남 할당한 공간 내에서 백그라운드 올바르게 작동되게 하자 
	// 그리고 이제 수정기능도 있으니까 수정한거를 저장하는 기능추가하기 

	ComponentTitle("Ani Load");
	ImGui::SameLine(100);
	
	static string AniName;

	AniName.resize(256);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##KeySet", AniName.data(), AniName.size());

	ImGui::SameLine();
	if (ImGui::Button("LOAD##123", ImVec2(40.f, 18.f)))
	{
		// 이전 데이터 정리
		if (m_PreviewObj != nullptr)
		{
			DestroyObject(m_PreviewObj);
			m_PreviewObj = nullptr;
		}

		// 기존 데이터 클리어
		m_CurSprite.clear();
		if (m_Flipbook != nullptr)
		{
			m_Flipbook = nullptr;
		}

		AniName = LoadFlipbook();
		m_PreviewObj = new CGameObject;
		m_PreviewObj->SetName(L"AnimationEdit");
		m_PreviewObj->AddComponent(new CMeshRender);
		m_PreviewObj->AddComponent(new CLight2D);
		m_PreviewObj->AddComponent(new CCollider2D);
		m_PreviewObj->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
		m_PreviewObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

		m_PreviewObj->Transform()->SetRelativePos(0.f, 0.f, 1.f);
		m_PreviewObj->Transform()->SetRelativeScale(300.f, 300.f, 1.f);
		m_PreviewObj->Collider2D()->SetScale(Vec2(0.8f, 0.8f));

		m_PreviewObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		m_PreviewObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
		m_PreviewObj->MeshRender()->GetDynamicMaterial();
		m_PreviewObj->AddComponent(new CFlipbookPlayer);
		m_PreviewObj->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(AniName.begin(), AniName.end())));
		m_PreviewObj->FlipbookPlayer()->Play(0, 8.f, true);

		CreateObject(m_PreviewObj, 4, false);
		

		m_CurSprite = m_Flipbook->GetVecSprite(); // 로드 했으면 멤버로 받아둠
		m_Idex = 0;

		// 비교용 스트링
		string Tempstr = " ";

		// 처음에 한번 보여줌 
		if (AniName.size() > Tempstr.size())
		{
			m_SpriteKey = string(m_CurSprite[m_Idex].Get()->GetKey().begin(), m_CurSprite[m_Idex].Get()->GetKey().end());

			// 텍스쳐 아틀라스 사이즈 받아옴 UV좌표를 픽셀 좌표로 바꾸기위함
			int AtlasWidth = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetWidth();
			int AtlasHeight = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetHeight();

			// 레프트탑 과 슬라이스 소수점 반올림
			for (size_t i = 0; i < m_CurSprite.size(); ++i)
			{
				Vec2 LT = m_CurSprite[i]->GetPixelLeftTop();
				Vec2 SL = m_CurSprite[i]->GetPixelSlice();
				LT.x = trunc(LT.x);
				LT.y = trunc(LT.y);
				SL.x = trunc(SL.x);
				SL.y = trunc(SL.y);

				m_CurSprite[i]->SetLeftTop(LT);
				m_CurSprite[i]->SetSlice(SL);
			}

			// 레프트 탑을 픽셀 좌표로 받아옴
			m_LeftTop = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetLeftTop().x,
					m_CurSprite[m_Idex].Get()->GetLeftTop().y)
			);

			// 슬라이스 값을 픽셀 좌표로 받아옴
			m_Slice = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetSlice().x,
					m_CurSprite[m_Idex].Get()->GetSlice().y)
			);

			// 백그라운드 값을 픽셀 좌표로 받아옴
			m_BackGround = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetBackground().x,
					m_CurSprite[m_Idex].Get()->GetBackground().y)
			);

			// 오프셋 값을 픽셀 좌표로 받아옴
			m_Offset = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetOffset().x,
					m_CurSprite[m_Idex].Get()->GetOffset().y)
			);

			// 현재 인덱스 정보로 스프라이트 이미지를 띄어줌 
			m_AniInfo.Texture = m_CurSprite[m_Idex].Get()->GetAtlasTexture();
			m_AniInfo.Texture_Resolution.x = (float)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetWidth();
			m_AniInfo.Texture_Resolution.y = (float)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetHeight();
			m_AniInfo.LeftTop = m_CurSprite[m_Idex].Get()->GetLeftTop();
			m_AniInfo.Slice = m_CurSprite[m_Idex].Get()->GetSlice();
			m_AniInfo.Offset = m_CurSprite[m_Idex].Get()->GetOffset();
			m_AniInfo.BackGround = m_CurSprite[m_Idex].Get()->GetBackground();
		}
	}

	if (ImGui::Button("Clear##123", ImVec2(40.f, 18.f)))
	{
		Clear();
	}

	// 로드도 했고 인덱스의 변화도 있으면 값 새로 세팅
	if (m_CurSprite.size() > 0) // 기본적으로 아직 로드를 안했으면 설정이 안되게함
	{
		// 텍스쳐 키값 받아옴
		m_SpriteKey = string(m_CurSprite[m_Idex].Get()->GetKey().begin(), m_CurSprite[m_Idex].Get()->GetKey().end());

		// 텍스쳐 아틀라스 사이즈 받아옴 UV좌표를 픽셀 좌표로 바꾸기위함
		int AtlasWidth = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetWidth();
		int AtlasHeight = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetHeight();

		// 레프트 탑을 픽셀 좌표로 받아옴
		m_LeftTop = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetLeftTop().x,
				m_CurSprite[m_Idex].Get()->GetLeftTop().y)
		);

		// 슬라이스 값을 픽셀 좌표로 받아옴
		m_Slice = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetSlice().x,
				m_CurSprite[m_Idex].Get()->GetSlice().y)
		);

		// 백그라운드 값을 픽셀 좌표로 받아옴
		m_BackGround = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetBackground().x,
				m_CurSprite[m_Idex].Get()->GetBackground().y)
		);

		// 오프셋 값을 픽셀 좌표로 받아옴
		m_Offset = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetOffset().x,
				m_CurSprite[m_Idex].Get()->GetOffset().y)
		);

		// 현재 인덱스 정보로 스프라이트 이미지를 띄어줌 
		m_AniInfo.Texture = m_CurSprite[m_Idex].Get()->GetAtlasTexture();
		m_AniInfo.Texture_Resolution.x = (float)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetWidth();
		m_AniInfo.Texture_Resolution.y = (float)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetHeight();
		m_AniInfo.LeftTop = m_CurSprite[m_Idex].Get()->GetLeftTop();
		m_AniInfo.Slice = m_CurSprite[m_Idex].Get()->GetSlice();
		m_AniInfo.Offset = m_CurSprite[m_Idex].Get()->GetOffset();
		m_AniInfo.BackGround = m_CurSprite[m_Idex].Get()->GetBackground();
	}
	
	ImGui::NewLine();
	ComponentTitle("Sprite Info");

	ImGui::Text("LeftTop");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragFloat("##LeftTopX", &m_LeftTop.x, 1.f))
	{
		m_LeftTop.x = trunc(m_LeftTop.x);

		// 픽셀 좌표 직접 전달
		m_CurSprite[m_Idex]->SetLeftTop(m_LeftTop);
	}

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragFloat("##LeftTopY", &m_LeftTop.y, 1.f))
	{
		m_LeftTop.y = trunc(m_LeftTop.y);

		m_CurSprite[m_Idex]->SetLeftTop(m_LeftTop);
	}

	ImGui::Text("Slice");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragFloat("##SliceX", &m_Slice.x, 1.f))
	{
		m_Slice.x = trunc(m_Slice.x);

		m_CurSprite[m_Idex]->SetSlice(m_Slice);
	}

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragFloat("##SliceY", &m_Slice.y, 1.f))
	{
		m_Slice.y = trunc(m_Slice.y);

		m_CurSprite[m_Idex]->SetSlice(m_Slice);
	}

	ImGui::Text("BackGround");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragFloat("##BackX", &m_BackGround.x, 10))
	{
		if (m_BackGround.x <= 10.f)
		{
			m_BackGround.x = 10.f;
		}

		m_BackGround.y = m_BackGround.x;

		for (size_t i = 0; i < m_CurSprite.size(); ++i)
		{
			m_CurSprite[i]->SetBackground(m_BackGround);
		}
	}
	

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragFloat("##BackY", &m_BackGround.y, 10))
	{
		if (m_BackGround.y <= 10.f)
		{
			m_BackGround.y = 10.f;
		}

		m_BackGround.x = m_BackGround.y;

		for (size_t i = 0; i < m_CurSprite.size(); ++i)
		{
			m_CurSprite[i]->SetBackground(m_BackGround);
		}
	}


	ImGui::Text("OffSet");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputFloat("##OffX", &m_Offset.x, 1))
	{
		m_CurSprite[m_Idex]->SetOffset(m_Offset);
	}

	ImGui::SameLine(220);
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputFloat("##OffY", &m_Offset.y, 1))
	{
		m_CurSprite[m_Idex]->SetOffset(m_Offset);
	}

	ImGui::Text("Sprite Name");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(135);


	// 선택한 인덱스의 스프라이트 키를 띄어줌
	if (ImGui::InputText("##KeySet22", m_SpriteKey.data(), m_SpriteKey.size() + 2))
	{
		m_CurSprite[m_Idex]->SetKey(wstring(m_SpriteKey.begin(), m_SpriteKey.end()));
	}

	m_PrevIdex = m_Idex;

	ImGui::SameLine();
	ImGui::SetNextItemWidth(77);
	if (ImGui::InputInt("##Index", &m_Idex, 1))
	{
		if (m_Idex < 0) // 인덱스 0이하로 못내려가게 막음 
		{
			m_Idex = 0;
		}
		else if (m_Idex >= m_Flipbook->GetMaxSprite()) //최대 범위도 스프라이트 범위로 막음
		{
			m_Idex -= 1;
		}
	}

	// 애니메이션 등록하고 파일로 저장시키는 기능
	ImGui::NewLine();
	if (ImGui::Button("EDIT##333", ImVec2(40.f, 18.f)))
	{
		SaveFlipbook(m_AniInfo.Texture);

		AniName = AniName.substr(0, AniName.find('\0')); // 실제 사이즈로 줄이기
		CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::FLIPBOOK ,wstring(AniName.begin(), AniName.end()));

		string Path = "Animation\\" + AniName + ".anim";
		Path = Path.substr(0, Path.find('\0'));
	
		CAssetMgr::GetInst()->LoadAni(wstring(Path.begin(), Path.end()));

		m_PreviewObj->FlipbookPlayer()->Stop();
		m_PreviewObj->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(AniName.begin(), AniName.end())));
		m_PreviewObj->FlipbookPlayer()->Play(0, 5.f, true);
	}

	// 스프라이트 프리뷰
	RenderSpritePreview();
}



void AnimationUI::RenderAtlasPreview()
{
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoMove;

	float displayWidth = 800.0f;
	float displayHeight = 690.0f;

	WheelCheck();

	ImGui::BeginChild("AtlasPreview", ImVec2(displayWidth, displayHeight), true, window_flags);

	if (m_AtlasInfo.AtlasTexture != nullptr)
	{
		float textureWidth = (float)m_AtlasInfo.AtlasTexture->GetWidth();
		float textureHeight = (float)m_AtlasInfo.AtlasTexture->GetHeight();

		// 화면에 맞는 기본 비율 계산
		float scaleX = displayWidth / textureWidth;
		float scaleY = displayHeight / textureHeight;
		float baseScale = min(scaleX, scaleY);  // 가로세로 비율 유지
		m_Ratio = baseScale * m_WheelScale;

		// 실제 이미지 크기
		float scaledWidth = textureWidth * m_Ratio;
		float scaledHeight = textureHeight * m_Ratio;

		// 뷰포트의 범위를 벗어나지 않도록 패닝 제한
		if (scaledWidth > displayWidth)
		{
			m_PanOffset.x = max(-(scaledWidth - displayWidth), min(0.f, m_PanOffset.x));
		}
		else
		{
			// 이미지가 화면보다 작으면 중앙 정렬
			m_PanOffset.x = (displayWidth - scaledWidth) * 0.5f;
		}

		if (scaledHeight > displayHeight)
		{
			m_PanOffset.y = max(-(scaledHeight - displayHeight), min(0.f, m_PanOffset.y));
		}
		else
		{
			// 이미지가 화면보다 작으면 중앙 정렬
			m_PanOffset.y = (displayHeight - scaledHeight) * 0.5f;
		}

		// 이미지 위치 설정
		ImGui::SetCursorPos(ImVec2(m_PanOffset.x, m_PanOffset.y));

		ImGui::Image(
			m_AtlasInfo.AtlasTexture->GetSRV().Get(),
			ImVec2(scaledWidth, scaledHeight),
			ImVec2(0, 0),
			ImVec2(1, 1));

		ImageRectMin = ImGui::GetItemRectMin();

		SelectCheck();

		// 선택 영역 표시 (패닝 고려함)
		//ImVec2 MouseLTPos = ImVec2(
		//	m_MouseLT.x * m_Ratio + ImageRectMin.x,
		//	m_MouseLT.y * m_Ratio + ImageRectMin.y
		//);
		//ImVec2 MouseRBPos = ImVec2(
		//	(m_MouseLT.x + m_Slice.x) * m_Ratio + ImageRectMin.x,
		//	(m_MouseLT.y + m_Slice.y) * m_Ratio + ImageRectMin.y
		//);
		ImVec2 MouseLTPos = ImVec2(
			m_LeftTop.x * m_Ratio + ImageRectMin.x,
			m_LeftTop.y * m_Ratio + ImageRectMin.y
		);
		ImVec2 MouseRBPos = ImVec2(
			(m_LeftTop.x + m_Slice.x) * m_Ratio + ImageRectMin.x,
			(m_LeftTop.y + m_Slice.y) * m_Ratio + ImageRectMin.y
		);

		ImGui::GetWindowDrawList()->AddRect(MouseLTPos, MouseRBPos,
			ImGui::GetColorU32(ImVec4(1.f, 1.f, 0.f, 1.f)), 0.f, 0.f, 1.f);
	}

	ImGui::EndChild();
}


void AnimationUI::RenderSpritePreview()
{
	ImGui::BeginChild("SpritePreview", ImVec2(340, 350), true);
	 
	if (m_AniInfo.Texture != nullptr)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 screenPos = ImGui::GetCursorScreenPos();

		// 화면에 표시될 크기 계산
		float displaySize = 340.0f;
		float scale = displaySize / m_AniInfo.BackGround.x;

		// 백그라운드 영역 계산
		Vec2 halfSlice = Vec2(m_AniInfo.Slice.x / 2.f, m_AniInfo.Slice.y / 2.f);
		Vec2 halfBackground = Vec2(m_AniInfo.BackGround.x / 2.f, m_AniInfo.BackGround.y / 2.f);

		Vec2 center = Vec2(
			m_AniInfo.LeftTop.x + halfSlice.x,
			m_AniInfo.LeftTop.y + halfSlice.y
		);

		Vec2 backgroundLeftTop = Vec2(
			center.x - halfBackground.x,
			center.y - halfBackground.y
		);

		Vec2 finalLeftTop = Vec2(
			backgroundLeftTop.x,
			backgroundLeftTop.y
		);

		Vec2 finalRightBottom = Vec2(
			finalLeftTop.x + m_AniInfo.BackGround.x,
			finalLeftTop.y + m_AniInfo.BackGround.y
		);

		// 슬라이스된 이미지가 화면에서 차지할 영역 계산
		float sliceWidth = m_AniInfo.Slice.x * scale;
		float sliceHeight = m_AniInfo.Slice.y * scale;
		float Temp = -0.000976562500 * scale;

		// 백그라운드 중심점 (화면 상의)
		float centerX = screenPos.x + (displaySize / 2.0f);
		float centerY = screenPos.y + (displaySize / 2.0f);

		// 오프셋 스케일 적용
		float scaledOffsetX = m_AniInfo.Offset.x * scale;
		float scaledOffsetY = m_AniInfo.Offset.y * scale;

		// 클리핑 영역 계산 (오프셋 적용)
		ImVec2 clipMin(
			centerX - (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY - (sliceHeight / 2.0f) + scaledOffsetY
		);
		ImVec2 clipMax(
			centerX + (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY + (sliceHeight / 2.0f) + scaledOffsetY
		);

		// 클리핑 영역 설정
		draw_list->PushClipRect(clipMin, clipMax, true);

		// 이미지 그리기 - 이미지 위치도 오프셋만큼 이동
		draw_list->AddImage(
			m_AniInfo.Texture->GetSRV().Get(),
			ImVec2(screenPos.x + scaledOffsetX + Temp, screenPos.y + scaledOffsetY), // 시작 위치에 오프셋 적용
			ImVec2(screenPos.x + displaySize + scaledOffsetX + Temp, screenPos.y + displaySize + scaledOffsetY), // 끝 위치에도 오프셋 적용
			ImVec2(finalLeftTop.x, finalLeftTop.y),
			ImVec2(finalRightBottom.x, finalRightBottom.y)
		);
		draw_list->PopClipRect();

		// 디버그용: 클리핑 영역 표시
		draw_list->AddRect(clipMin, clipMax, IM_COL32(255, 0, 0, 255));
	}

	ImGui::EndChild();
}

void AnimationUI::RenderSpritePreview2()
{
	// Child 윈도우의 배경색 설정
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));

	ImGui::BeginChild("##SpritePreview2", ImVec2(390, 410), true);

	if (m_AniInfo.Texture != nullptr)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 screenPos = ImGui::GetCursorScreenPos();

		// 화면에 표시될 크기 계산
		float displaySize = 390.f;
		float scale = displaySize / m_AniInfo.BackGround.x;

		// 백그라운드 영역 계산
		Vec2 halfSlice = Vec2(m_AniInfo.Slice.x / 2.f, m_AniInfo.Slice.y / 2.f);
		Vec2 halfBackground = Vec2(m_AniInfo.BackGround.x / 2.f, m_AniInfo.BackGround.y / 2.f);

		Vec2 center = Vec2(
			m_AniInfo.LeftTop.x + halfSlice.x,
			m_AniInfo.LeftTop.y + halfSlice.y
		);

		Vec2 backgroundLeftTop = Vec2(
			center.x - halfBackground.x,
			center.y - halfBackground.y
		);

		Vec2 finalLeftTop = Vec2(
			backgroundLeftTop.x,
			backgroundLeftTop.y
		);

		Vec2 finalRightBottom = Vec2(
			finalLeftTop.x + m_AniInfo.BackGround.x,
			finalLeftTop.y + m_AniInfo.BackGround.y
		);

		// 슬라이스된 이미지가 화면에서 차지할 영역 계산
		float sliceWidth = m_AniInfo.Slice.x * scale;
		float sliceHeight = m_AniInfo.Slice.y * scale;
		float Temp = -0.000976562500 * scale;

		// 백그라운드 중심점 (화면 상의)
		float centerX = screenPos.x + (displaySize / 2.0f);
		float centerY = screenPos.y + (displaySize / 2.0f);

		// 오프셋 스케일 적용
		float scaledOffsetX = m_AniInfo.Offset.x * scale;
		float scaledOffsetY = m_AniInfo.Offset.y * scale;

		// 클리핑 영역 계산 (오프셋 적용)
		ImVec2 clipMin(
			centerX - (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY - (sliceHeight / 2.0f) + scaledOffsetY
		);
		ImVec2 clipMax(
			centerX + (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY + (sliceHeight / 2.0f) + scaledOffsetY
		);

		// 클리핑 영역 설정
		draw_list->PushClipRect(clipMin, clipMax, true);

		// 이미지 그리기 - 이미지 위치도 오프셋만큼 이동
		draw_list->AddImage(
			m_AniInfo.Texture->GetSRV().Get(),
			ImVec2(screenPos.x + scaledOffsetX + Temp, screenPos.y + scaledOffsetY),         // 시작 위치에 오프셋 적용
			ImVec2(screenPos.x + displaySize + scaledOffsetX + Temp, screenPos.y + displaySize + scaledOffsetY),  // 끝 위치에도 오프셋 적용
			ImVec2(finalLeftTop.x, finalLeftTop.y),
			ImVec2(finalRightBottom.x, finalRightBottom.y)
		);
		draw_list->PopClipRect();

		// 디버그용: 클리핑 영역 표시
		draw_list->AddRect(clipMin, clipMax, IM_COL32(255, 0, 0, 255));

		RenderDrawLine();
	}


	ImGui::EndChild();
	ImGui::PopStyleColor();  // 스타일 복원

}

void AnimationUI::RenderAnimationPreview()
{
	// Child 윈도우의 배경색 설정
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
	ImGui::BeginChild("AnimationPreview##2", ImVec2(390, 410), true);
	static bool Play = true;

	if (m_Flipbook != nullptr)
	{
		vector<Ptr<CSprite>> vecSprite = m_Flipbook->GetVecSprite();
		if (vecSprite.size() >= 1)
		{
			float Term = (1.f / m_FPS);

			if (Term < m_Time && Play)
			{
				++m_SpriteIdx;
				m_Time -= Term;
			}

			if (m_Flipbook->GetMaxSprite() <= m_SpriteIdx)
			{
				m_SpriteIdx = 0;
			}

			Ptr<CSprite> currentSprite = vecSprite[m_SpriteIdx];
			Ptr<CTexture> SpTexture = currentSprite->GetAtlasTexture();

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 screenPos = ImGui::GetCursorScreenPos();

			ImVec2 windowSize = ImGui::GetWindowSize();	// 추가

			// 화면에 표시될 크기 계산
			float displaySize = 390.f;
			float scale = displaySize / currentSprite->GetBackground().x;

			// 백그라운드 영역 계산
			Vec2 halfSlice = Vec2(currentSprite->GetSlice().x / 2.f, currentSprite->GetSlice().y / 2.f);
			Vec2 halfBackground = Vec2(currentSprite->GetBackground().x / 2.f, currentSprite->GetBackground().y / 2.f);
			Vec2 center = Vec2(
				currentSprite->GetLeftTop().x + halfSlice.x,
				currentSprite->GetLeftTop().y + halfSlice.y
			);
			Vec2 backgroundLeftTop = Vec2(
				center.x - halfBackground.x,
				center.y - halfBackground.y
			);
			Vec2 finalLeftTop = Vec2(
				backgroundLeftTop.x,
				backgroundLeftTop.y
			);
			Vec2 finalRightBottom = Vec2(
				finalLeftTop.x + currentSprite->GetBackground().x,
				finalLeftTop.y + currentSprite->GetBackground().y
			);

			// 슬라이스된 이미지가 화면에서 차지할 영역 계산
			float sliceWidth = currentSprite->GetSlice().x * scale;
			float sliceHeight = currentSprite->GetSlice().y * scale;

			// 백그라운드 중심점 (화면 상의)
			float centerX = screenPos.x + (displaySize / 2.0f);
			float centerY = screenPos.y + (displaySize / 2.0f);

			// 오프셋 스케일 적용
			float scaledOffsetX = currentSprite->GetOffset().x * scale;
			float scaledOffsetY = currentSprite->GetOffset().y * scale;
			float Temp = -0.000976562500 * scale;

			// 클리핑 영역 계산 (오프셋 적용)
			ImVec2 clipMin(
				centerX - (sliceWidth / 2.0f) + scaledOffsetX + Temp,
				centerY - (sliceHeight / 2.0f) + scaledOffsetY
			);
			ImVec2 clipMax(
				centerX + (sliceWidth / 2.0f) + scaledOffsetX + Temp,
				centerY + (sliceHeight / 2.0f) + scaledOffsetY
			);

			// 클리핑 영역 설정
			draw_list->PushClipRect(clipMin, clipMax, true);

			// 이미지 그리기 - 이미지 위치도 오프셋만큼 이동
			draw_list->AddImage(
				SpTexture->GetSRV().Get(),
				ImVec2(screenPos.x + scaledOffsetX , screenPos.y + scaledOffsetY + Temp),
				ImVec2(screenPos.x + displaySize + scaledOffsetX + Temp, screenPos.y + displaySize + scaledOffsetY),
				ImVec2(finalLeftTop.x, finalLeftTop.y),
				ImVec2(finalRightBottom.x, finalRightBottom.y));

			draw_list->PopClipRect();

			// 디버그용: 클리핑 영역 표시
			draw_list->AddRect(clipMin, clipMax, IM_COL32(255, 0, 0, 255));

			RenderDrawLine();
			 
			if (Play)
				m_Time += EngineDT;
		}
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();  // 스타일 복원

	ImGui::SameLine();
	RenderSpritePreview2();

	if (ImGui::Button("Play", ImVec2(40.f, 20.f)))
	{
		Play = true;

		if (m_PreviewObj != nullptr)
		{
			m_PreviewObj->FlipbookPlayer()->Play(0.f, 8.f, true);
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(40.f, 20.f)))
	{
		Play = false;

		if (m_PreviewObj != nullptr)
		{
			m_PreviewObj->FlipbookPlayer()->Pause();
		}
	}

	int* Idx = &m_SpriteIdx;

	float wheel = ImGui::GetIO().MouseWheel;

	if (wheel != 0)
	{
		const float WHEEL_SENSITIVITY = 1.f;
		*Idx += (int)(wheel * WHEEL_SENSITIVITY);
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(70.f);
	ImGui::InputInt("##IDX", Idx, 1);
}

void AnimationUI::RenderDrawLine()
{
	// 중심선 그리기
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	// 수직선
	ImVec2 lineStartY(windowPos.x + windowSize.x / 2, windowPos.y);
	ImVec2 lineEndY(windowPos.x + windowSize.x / 2, windowPos.y + windowSize.y);
	lineStartY.x += 1.f;
	lineEndY.x += 1.f;
	drawList->AddLine(lineStartY, lineEndY, IM_COL32(0, 125, 0, 125), 1.0f);

	float Line = 50.f;
	float Limit = 125.f;

	for (int i = 0; i < 4; ++i)
	{
		// 수평선
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}

	for (int i = 0; i < 4; ++i)
	{
		// 수평선
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 + (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit, windowPos.y + windowSize.y / 2 + (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}


	Line = 25.f;
	Limit = 150.f;

	for (int i = 1; i < 7; ++i)
	{
		// 수평선
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}

	for (int i = 1; i < 7; ++i)
	{
		// 수평선
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 + (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit , windowPos.y + windowSize.y / 2 + (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}

}

void AnimationUI::WheelCheck()
{
	if (0 < ImGui::GetIO().MouseWheel)
	{
		m_WheelScale += 0.05f;
	}

	if (0 > ImGui::GetIO().MouseWheel)
	{
		m_WheelScale -= 0.05f;
	}

	if (3.f < m_WheelScale)
		m_WheelScale = 5.f;
	if (m_WheelScale < 0.1f)
		m_WheelScale = 0.1f;

}

void AnimationUI::SelectCheck()
{
	ImageRectMin = ImGui::GetItemRectMin();
	ImageRectMax = ImVec2(
		ImageRectMin.x + ImGui::GetItemRectSize().x,
		ImageRectMin.y + ImGui::GetItemRectSize().y
	);
	m_MousePos = ImGui::GetMousePos();

	// ImGui의 IsItemHovered()를 사용하여 이미지 위에 마우스가 있는지 확인
	bool isMouseInImage = ImGui::IsItemHovered();

	// 패닝 시작 - 이미지 위에서 좌클릭할 때만
	if (!m_IsPanning && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isMouseInImage)
	{
		m_IsPanning = true;
		m_LastMousePos = m_MousePos;
	}
	// 패닝 중 - 드래그 중일 때
	else if (m_IsPanning && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		float deltaX = m_MousePos.x - m_LastMousePos.x;
		float deltaY = m_MousePos.y - m_LastMousePos.y;

		m_PanOffset.x += deltaX;
		m_PanOffset.y += deltaY;

		m_LastMousePos = m_MousePos;
	}
	// 패닝 종료
	else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		m_IsPanning = false;
	}

	// 우클릭으로 스프라이트 선택 - 이미지 위에서만 동작
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && isMouseInImage)
	{
		Vec2 vPixelPos = Vec2(
			(m_MousePos.x - ImageRectMin.x) / m_Ratio,
			(m_MousePos.y - ImageRectMin.y) / m_Ratio
		);

		if (0.f <= vPixelPos.x && vPixelPos.x < m_AtlasInfo.AtlasTexture->GetWidth()
			&& 0.f <= vPixelPos.y && vPixelPos.y < m_AtlasInfo.AtlasTexture->GetHeight())
		{
			CalcSpriteSize(vPixelPos);
		}
	}
}

void AnimationUI::CalcSpriteSize(Vec2 _PixelPos)
{
	if (!IsPixelOk(_PixelPos))
		return;

	int width = m_AtlasInfo.AtlasTexture->GetWidth();
	int height = m_AtlasInfo.AtlasTexture->GetHeight();

	int startX = (int)_PixelPos.x;
	int startY = (int)_PixelPos.y;

	if (startX < 0 || startX >= width || startY < 0 || startY >= height)
		return;

	const int dx[] = { -1, -1, -1,  0,  0,  1,  1,  1 };
	const int dy[] = { -1,  0,  1, -1,  1, -1,  0,  1 };

	int minX = startX;
	int maxX = startX;
	int minY = startY;
	int maxY = startY;

	m_PixelID.clear();
	list<Vec2> checkList;

	checkList.push_back(_PixelPos);
	m_PixelID.insert(_PixelPos);

	while (!checkList.empty())
	{
		Vec2 current = checkList.front();
		checkList.pop_front();

		int x = (int)current.x;
		int y = (int)current.y;

		if (m_AtlasInfo.AtlasTexture->GetPixels()[width * y + x].a > 0.0f)
		{
			minX = min(minX, x);
			maxX = max(maxX, x);
			minY = min(minY, y);
			maxY = max(maxY, y);

			// 8방향 검사
			for (int i = 0; i < 8; ++i)
			{
				int nx = x + dx[i];
				int ny = y + dy[i];

				if (nx < 0 || nx >= width || ny < 0 || ny >= height)
					continue;

				Vec2 nextPos((float)nx, (float)ny);
				if (m_PixelID.find(nextPos) == m_PixelID.end())
				{
					// 알파값이 있는 픽셀만 추가
					if (m_AtlasInfo.AtlasTexture->GetPixels()[width * ny + nx].a > 0.0f)
					{
						checkList.push_back(nextPos);
						m_PixelID.insert(nextPos);
					}
				}
			}
		}
	}

	m_LeftTop = Vec2((float)minX, (float)minY);
	m_Slice = Vec2((float)(maxX - minX + 1), (float)(maxY - minY + 1));
	m_MouseLT = ImVec2((float)minX, (float)minY);
	m_MouseRB = ImVec2((float)maxX, (float)maxY);
}

bool AnimationUI::IsPixelOk(Vec2 _PixelPos)
{
	int width = m_AtlasInfo.AtlasTexture->GetWidth();
	int height = m_AtlasInfo.AtlasTexture->GetHeight();

	int x = (int)_PixelPos.x;
	int y = (int)_PixelPos.y;

	if (x < 0 || x >= width || y < 0 || y >= height)
		return false;

	if (m_PixelID.find(_PixelPos) != m_PixelID.end())
		return false;

	int pixelIdx = width * y + x;
	return m_AtlasInfo.AtlasTexture->GetPixels()[pixelIdx].a > 0.0f;

}