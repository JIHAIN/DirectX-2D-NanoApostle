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
	// ���̾� ���� �޺� �ڽ�
	// �׽�ũ �Ŵ��� ó�� �ڱ� ���ڵ� �� �ʱ�ȭ ��Ű�� ����ƽ�� ����ǰ� ����

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

	// ��������Ʈ ������ â
	//RenderSpritePreview();

	// �ִϸ��̼� ������ â
	//RenderAnimationPreview();

	// ������ �Ϸ� ������ �ø����� �����ϰ� ���� new �� ���ο� �ø����� ���������� ������ �����ڿ��� 
	// new �Ҵ��� �ϱ� ������ �ϳ��� �ø��� �ۿ� ������ ����. 
	// �׸��� �ؽ��� �ε� �ϸ� ���� ���ȭ�鿡 �ؽ��� ������ �׸��� ���� �����Ѱ� �� �� �ְ� ������� 
	// ������ �� �ְ� �׸��� ���� �� �̹� ����� �ٽ� �� �� �ְԵ� ��������.
	// ������ ������ �ƴٰ� ǥ�� �Ǹ� ���� �� ����.
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

	// �� ��� �� �ʱ�ȭ�ϱ�
	m_Flipbook = nullptr;
	m_Texture = nullptr;
	m_AniInfo = {};
	m_AtlasInfo = {};
	m_CurSprite.clear();

	m_TextureKey = " ";
	m_TexturePath = " ";

	m_FlibookKey = " ";
	m_FlibookKeyBuffer.assign(m_FlibookKeyBuffer.size(), '\0');

	m_Idex = 0; //�������̺z �ε��� ����� Ű���� �̸����� �̰� ����
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
			wstring selectedFile = szFilePath;

			// �� ���� ����
			wstring fileName = selectedFile.substr(selectedFile.find_last_of(L"\\") + 1);

			wstring relativePath = L"Texture\\" + fileName;
			m_Texture = CAssetMgr::GetInst()->Load<CTexture>(relativePath, relativePath);
			return string(relativePath.begin(), relativePath.end());
		}
	}
	// �Լ��� ��ȯ Ÿ���� string �̹Ƿ� ��� ���ǿ� �ɸ��� ���� ��� �⺻���� �� ���ڿ� ��ȯ


	return "";
}


int AnimationUI::SaveFlipbook(Ptr<CTexture> _Texture)
{
	wstring Path = CPathMgr::GetInst()->GetContentPath();
	wstring strName = L" ";
	strName = m_Flipbook->GetName();
	

	// ��ü ��θ� �ѹ��� ����
	wstring FilePath = Path + L"Animation\\" + strName + L".anim";

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, FilePath.c_str(), L"w");

	if (!pFile)
	{
		// ���� ���⿡ ������ ���
		MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
		return FALSE; // �Լ� ����
	}

	// �ִϸ��̼� �̸�
	fwprintf(pFile, L"[ANIMATION_NAME]\n");
	fwprintf(pFile, L"%s\n\n", strName.c_str());

	// �����ϴ� ��Ʋ�� �ؽ��� ����
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
	
	// ���� ������ �ø����� ������ ���� ��������Ʈ �޾ƿͼ� �ű⿡ ����� ���� ��ŭ ���Ϸ� ������.
	vector<Ptr<CSprite>> m_vecSprite = m_Flipbook->GetVecSprite();

	// �� �����Ӻ� ������
	fwprintf(pFile, L"[FRAME_DATA]\n");
	fwprintf(pFile, L"Frame_Count %d\n\n", static_cast<int>(m_vecSprite.size()));

	// �ؽ��� ũ�� ����
	int TWidth = _Texture.Get()->GetWidth();
	int THeight = _Texture.Get()->GetHeight();

	for (size_t i = 0; i < m_vecSprite.size(); ++i)
	{
		// UV ��ǥ�� �ȼ� ��ǥ�� ��ȯ�ؼ� �����Ŵ
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
	// ���� Ž���� ����
	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // �θ� ������ �ڵ��� �ִٸ� ���⿡ ����
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All Files (*.*)\0*.*\0Animation Files (*.anim)\0*.anim\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	wstring Path = CPathMgr::GetInst()->GetContentPath();
	Path += L"Animation";


	// �ʱ� ���丮 ����
	wchar_t initialDir[MAX_PATH];
	wcscpy_s(initialDir, Path.c_str());
	
	ofn.lpstrInitialDir = initialDir;

	 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		// ���� ����
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, szFile, L"r");

		if (!pFile)
		{
			// ���� ���⿡ ������ ���
			MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
			return " "; // �Լ� ����
		}

		wchar_t szRead[255] = {};
		wstring AniName;
		Ptr<CTexture> EditAtlas = nullptr; 	// ������ �ؽ��� �̸� ����
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
					// �ؽ��� ã�Ƽ� �޾Ƶ�
					EditAtlas = CAssetMgr::GetInst()->Load<CTexture>(strKey, strRelativePath);
				}
			}
			else if (str == L"[FRAME_DATA]")
			{
				wchar_t FrameBuff[255] = {};

				int frameCount = 0;
				// ���� ���� ó��
				fgetwc(pFile);
				
				fwscanf_s(pFile, L"Frame_Count %d", &frameCount);

				//SpriteInfo �� ī��Ʈ ����ŭ �غ���
				vector<SpriteInfo> VecSpInfo;
				SpriteInfo SInfo = { };

				//Ű�� �� �޾Ƶ� 
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

				for (int i = 0; i < frameCount; ++i) // ã�� ������ ����� 
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

		// flipbook ���
		CAssetMgr::GetInst()->AddAsset(m_Flipbook->GetName(), m_Flipbook);

		fclose(pFile);

		return string(m_Flipbook->GetName().begin(), m_Flipbook->GetName().end());
	}
	else
	{
		// ���� ������ ����� ���
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
		m_TextureKey = SelectAtlas(); // ����Ž����� �ؽ��� �ҷ��� �� �ְ���

		if (m_Texture != nullptr)
		{
			float TextureX = (float)m_Texture.Get()->GetWidth();
			float TextureY = (float)m_Texture.Get()->GetHeight();

			DebugRectScale = Vec2(TextureX, TextureY); // ����� �׸���� ��ġ �Ѱ��ֱ�

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
		DebugRectScale = Vec2(0.f, 0.f); // �ؽ��� �����ÿ��� �ʱ�ȭ 

	ComponentTitle("Sprite Set");

	// �ϳ��� �غ��� 
	// ���콺 ��ġ �޾ƿ��°� ���� ����
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

	// ��������� ���콺�� �̿��ؼ� �»�� , �����̽� �� ���ϱ� �� ����׷� ǥ���ϱ� ��׶��� ���� ������ ����

	Ptr<CSprite> pSprite = nullptr;  // ��������Ʈ �غ� 

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

	if (KEY_TAP(KEY::ENTER) || ImGui::Button("ADD##123", ImVec2(40.f, 18.f))) // ��ư�� ������ ���� ������ �����Ѱ͵��� �־���
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(m_Texture);
		pSprite->SetLeftTop(m_LeftTop);
		pSprite->SetSlice(m_Slice);
		pSprite->SetBackground(m_BackGround);
		pSprite->SetOffset(m_Offset);

		++m_Idex;

		wstring szKey = {}; // Ű�� �����
		szKey = wstring(m_SpriteKey.begin(), m_SpriteKey.end());

		szKey += L"_" + std::to_wstring(m_Idex); // �ڿ� ����ٿ� �ε��� ��ȣ �ֱ�

		CAssetMgr::GetInst()->AddAsset(szKey, pSprite);

		// ��������Ʈ ���� ����ϰ� �� ������ â�� ����
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
		// string���� ��ȯ (�ι��ڱ�����)
		m_FlibookKey = string(m_FlibookKeyBuffer.data());

		// wstring���� ��ȯ
		m_Flipbook->SetName(wstring(m_FlibookKey.begin(), m_FlibookKey.end()));
	}

	ImGui::SameLine();
	if (ImGui::Button("SAVE##323", ImVec2(40.f, 18.f)))
	{
		CAssetMgr::GetInst()->AddAsset(wstring(m_FlibookKey.begin(), m_FlibookKey.end()), m_Flipbook);
		m_Idex = 0;
		//////////////////
		vector<Ptr<CSprite>> SpriteOffsetting = m_Flipbook->GetVecSprite();

		// ù ��° �������� �� ��ġ�� �������� ���
		Ptr<CSprite> baseSprite = SpriteOffsetting[0];
		float baseSliceHeight = baseSprite->GetPixelSlice().y;
		float baseBackgroundHeight = baseSprite->GetPixelBackground().y;

		// ���� �����ӿ��� �����̽��� ��׶��� ������ ��ġ�ϴ� �߾� Y��ġ
		float baseCenter = (baseBackgroundHeight - baseSliceHeight) * 0.5f;

		for (size_t i = 1; i < SpriteOffsetting.size(); ++i) 
		{

			Ptr<CSprite> sprite = SpriteOffsetting[i];
			float sliceHeight = sprite->GetPixelSlice().y;
			float backgroundHeight = sprite->GetPixelBackground().y;

			// ���� �������� �߾� Y��ġ
			float currentCenter = (backgroundHeight - sliceHeight) * 0.5f;

			// ���� �����Ӱ��� ���̸� ���������� ����
			float offset = baseCenter - currentCenter;

			sprite->SetOffset(Vec2(0.f, -offset));
		}

		////////////////
		SaveFlipbook(m_Texture);

		m_Flipbook = nullptr;
	}

	// ��������Ʈ ������ â
	RenderSpritePreview();
}

void AnimationUI::EditMode()
{
	// �ؾ��� �� ��׶��� ������Ѽ� ���� �����ϴ°� �ùٸ��� ���̰� �ؾ���
	// ������ �����ص� Ƽ�� �ȳ� �Ҵ��� ���� ������ ��׶��� �ùٸ��� �۵��ǰ� ���� 
	// �׸��� ���� ������ɵ� �����ϱ� �����ѰŸ� �����ϴ� ����߰��ϱ� 

	ComponentTitle("Ani Load");
	ImGui::SameLine(100);
	
	static string AniName;

	AniName.resize(256);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##KeySet", AniName.data(), AniName.size());

	ImGui::SameLine();
	if (ImGui::Button("LOAD##123", ImVec2(40.f, 18.f)))
	{
		// ���� ������ ����
		if (m_PreviewObj != nullptr)
		{
			DestroyObject(m_PreviewObj);
			m_PreviewObj = nullptr;
		}

		// ���� ������ Ŭ����
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
		

		m_CurSprite = m_Flipbook->GetVecSprite(); // �ε� ������ ����� �޾Ƶ�
		m_Idex = 0;

		// �񱳿� ��Ʈ��
		string Tempstr = " ";

		// ó���� �ѹ� ������ 
		if (AniName.size() > Tempstr.size())
		{
			m_SpriteKey = string(m_CurSprite[m_Idex].Get()->GetKey().begin(), m_CurSprite[m_Idex].Get()->GetKey().end());

			// �ؽ��� ��Ʋ�� ������ �޾ƿ� UV��ǥ�� �ȼ� ��ǥ�� �ٲٱ�����
			int AtlasWidth = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetWidth();
			int AtlasHeight = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetHeight();

			// ����Ʈž �� �����̽� �Ҽ��� �ݿø�
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

			// ����Ʈ ž�� �ȼ� ��ǥ�� �޾ƿ�
			m_LeftTop = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetLeftTop().x,
					m_CurSprite[m_Idex].Get()->GetLeftTop().y)
			);

			// �����̽� ���� �ȼ� ��ǥ�� �޾ƿ�
			m_Slice = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetSlice().x,
					m_CurSprite[m_Idex].Get()->GetSlice().y)
			);

			// ��׶��� ���� �ȼ� ��ǥ�� �޾ƿ�
			m_BackGround = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetBackground().x,
					m_CurSprite[m_Idex].Get()->GetBackground().y)
			);

			// ������ ���� �ȼ� ��ǥ�� �޾ƿ�
			m_Offset = UVToPixel(AtlasWidth, AtlasHeight,
				Vec2(m_CurSprite[m_Idex].Get()->GetOffset().x,
					m_CurSprite[m_Idex].Get()->GetOffset().y)
			);

			// ���� �ε��� ������ ��������Ʈ �̹����� ����� 
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

	// �ε嵵 �߰� �ε����� ��ȭ�� ������ �� ���� ����
	if (m_CurSprite.size() > 0) // �⺻������ ���� �ε带 �������� ������ �ȵǰ���
	{
		// �ؽ��� Ű�� �޾ƿ�
		m_SpriteKey = string(m_CurSprite[m_Idex].Get()->GetKey().begin(), m_CurSprite[m_Idex].Get()->GetKey().end());

		// �ؽ��� ��Ʋ�� ������ �޾ƿ� UV��ǥ�� �ȼ� ��ǥ�� �ٲٱ�����
		int AtlasWidth = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetWidth();
		int AtlasHeight = (int)m_CurSprite[m_Idex].Get()->GetAtlasTexture()->GetHeight();

		// ����Ʈ ž�� �ȼ� ��ǥ�� �޾ƿ�
		m_LeftTop = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetLeftTop().x,
				m_CurSprite[m_Idex].Get()->GetLeftTop().y)
		);

		// �����̽� ���� �ȼ� ��ǥ�� �޾ƿ�
		m_Slice = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetSlice().x,
				m_CurSprite[m_Idex].Get()->GetSlice().y)
		);

		// ��׶��� ���� �ȼ� ��ǥ�� �޾ƿ�
		m_BackGround = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetBackground().x,
				m_CurSprite[m_Idex].Get()->GetBackground().y)
		);

		// ������ ���� �ȼ� ��ǥ�� �޾ƿ�
		m_Offset = UVToPixel(AtlasWidth, AtlasHeight,
			Vec2(m_CurSprite[m_Idex].Get()->GetOffset().x,
				m_CurSprite[m_Idex].Get()->GetOffset().y)
		);

		// ���� �ε��� ������ ��������Ʈ �̹����� ����� 
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

		// �ȼ� ��ǥ ���� ����
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


	// ������ �ε����� ��������Ʈ Ű�� �����
	if (ImGui::InputText("##KeySet22", m_SpriteKey.data(), m_SpriteKey.size() + 2))
	{
		m_CurSprite[m_Idex]->SetKey(wstring(m_SpriteKey.begin(), m_SpriteKey.end()));
	}

	m_PrevIdex = m_Idex;

	ImGui::SameLine();
	ImGui::SetNextItemWidth(77);
	if (ImGui::InputInt("##Index", &m_Idex, 1))
	{
		if (m_Idex < 0) // �ε��� 0���Ϸ� ���������� ���� 
		{
			m_Idex = 0;
		}
		else if (m_Idex >= m_Flipbook->GetMaxSprite()) //�ִ� ������ ��������Ʈ ������ ����
		{
			m_Idex -= 1;
		}
	}

	// �ִϸ��̼� ����ϰ� ���Ϸ� �����Ű�� ���
	ImGui::NewLine();
	if (ImGui::Button("EDIT##333", ImVec2(40.f, 18.f)))
	{
		SaveFlipbook(m_AniInfo.Texture);

		AniName = AniName.substr(0, AniName.find('\0')); // ���� ������� ���̱�
		CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::FLIPBOOK ,wstring(AniName.begin(), AniName.end()));

		string Path = "Animation\\" + AniName + ".anim";
		Path = Path.substr(0, Path.find('\0'));
	
		CAssetMgr::GetInst()->LoadAni(wstring(Path.begin(), Path.end()));

		m_PreviewObj->FlipbookPlayer()->Stop();
		m_PreviewObj->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(AniName.begin(), AniName.end())));
		m_PreviewObj->FlipbookPlayer()->Play(0, 5.f, true);
	}

	// ��������Ʈ ������
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

		// ȭ�鿡 �´� �⺻ ���� ���
		float scaleX = displayWidth / textureWidth;
		float scaleY = displayHeight / textureHeight;
		float baseScale = min(scaleX, scaleY);  // ���μ��� ���� ����
		m_Ratio = baseScale * m_WheelScale;

		// ���� �̹��� ũ��
		float scaledWidth = textureWidth * m_Ratio;
		float scaledHeight = textureHeight * m_Ratio;

		// ����Ʈ�� ������ ����� �ʵ��� �д� ����
		if (scaledWidth > displayWidth)
		{
			m_PanOffset.x = max(-(scaledWidth - displayWidth), min(0.f, m_PanOffset.x));
		}
		else
		{
			// �̹����� ȭ�麸�� ������ �߾� ����
			m_PanOffset.x = (displayWidth - scaledWidth) * 0.5f;
		}

		if (scaledHeight > displayHeight)
		{
			m_PanOffset.y = max(-(scaledHeight - displayHeight), min(0.f, m_PanOffset.y));
		}
		else
		{
			// �̹����� ȭ�麸�� ������ �߾� ����
			m_PanOffset.y = (displayHeight - scaledHeight) * 0.5f;
		}

		// �̹��� ��ġ ����
		ImGui::SetCursorPos(ImVec2(m_PanOffset.x, m_PanOffset.y));

		ImGui::Image(
			m_AtlasInfo.AtlasTexture->GetSRV().Get(),
			ImVec2(scaledWidth, scaledHeight),
			ImVec2(0, 0),
			ImVec2(1, 1));

		ImageRectMin = ImGui::GetItemRectMin();

		SelectCheck();

		// ���� ���� ǥ�� (�д� �����)
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

		// ȭ�鿡 ǥ�õ� ũ�� ���
		float displaySize = 340.0f;
		float scale = displaySize / m_AniInfo.BackGround.x;

		// ��׶��� ���� ���
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

		// �����̽��� �̹����� ȭ�鿡�� ������ ���� ���
		float sliceWidth = m_AniInfo.Slice.x * scale;
		float sliceHeight = m_AniInfo.Slice.y * scale;
		float Temp = -0.000976562500 * scale;

		// ��׶��� �߽��� (ȭ�� ����)
		float centerX = screenPos.x + (displaySize / 2.0f);
		float centerY = screenPos.y + (displaySize / 2.0f);

		// ������ ������ ����
		float scaledOffsetX = m_AniInfo.Offset.x * scale;
		float scaledOffsetY = m_AniInfo.Offset.y * scale;

		// Ŭ���� ���� ��� (������ ����)
		ImVec2 clipMin(
			centerX - (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY - (sliceHeight / 2.0f) + scaledOffsetY
		);
		ImVec2 clipMax(
			centerX + (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY + (sliceHeight / 2.0f) + scaledOffsetY
		);

		// Ŭ���� ���� ����
		draw_list->PushClipRect(clipMin, clipMax, true);

		// �̹��� �׸��� - �̹��� ��ġ�� �����¸�ŭ �̵�
		draw_list->AddImage(
			m_AniInfo.Texture->GetSRV().Get(),
			ImVec2(screenPos.x + scaledOffsetX + Temp, screenPos.y + scaledOffsetY), // ���� ��ġ�� ������ ����
			ImVec2(screenPos.x + displaySize + scaledOffsetX + Temp, screenPos.y + displaySize + scaledOffsetY), // �� ��ġ���� ������ ����
			ImVec2(finalLeftTop.x, finalLeftTop.y),
			ImVec2(finalRightBottom.x, finalRightBottom.y)
		);
		draw_list->PopClipRect();

		// ����׿�: Ŭ���� ���� ǥ��
		draw_list->AddRect(clipMin, clipMax, IM_COL32(255, 0, 0, 255));
	}

	ImGui::EndChild();
}

void AnimationUI::RenderSpritePreview2()
{
	// Child �������� ���� ����
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));

	ImGui::BeginChild("##SpritePreview2", ImVec2(390, 410), true);

	if (m_AniInfo.Texture != nullptr)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 screenPos = ImGui::GetCursorScreenPos();

		// ȭ�鿡 ǥ�õ� ũ�� ���
		float displaySize = 390.f;
		float scale = displaySize / m_AniInfo.BackGround.x;

		// ��׶��� ���� ���
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

		// �����̽��� �̹����� ȭ�鿡�� ������ ���� ���
		float sliceWidth = m_AniInfo.Slice.x * scale;
		float sliceHeight = m_AniInfo.Slice.y * scale;
		float Temp = -0.000976562500 * scale;

		// ��׶��� �߽��� (ȭ�� ����)
		float centerX = screenPos.x + (displaySize / 2.0f);
		float centerY = screenPos.y + (displaySize / 2.0f);

		// ������ ������ ����
		float scaledOffsetX = m_AniInfo.Offset.x * scale;
		float scaledOffsetY = m_AniInfo.Offset.y * scale;

		// Ŭ���� ���� ��� (������ ����)
		ImVec2 clipMin(
			centerX - (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY - (sliceHeight / 2.0f) + scaledOffsetY
		);
		ImVec2 clipMax(
			centerX + (sliceWidth / 2.0f) + scaledOffsetX + Temp,
			centerY + (sliceHeight / 2.0f) + scaledOffsetY
		);

		// Ŭ���� ���� ����
		draw_list->PushClipRect(clipMin, clipMax, true);

		// �̹��� �׸��� - �̹��� ��ġ�� �����¸�ŭ �̵�
		draw_list->AddImage(
			m_AniInfo.Texture->GetSRV().Get(),
			ImVec2(screenPos.x + scaledOffsetX + Temp, screenPos.y + scaledOffsetY),         // ���� ��ġ�� ������ ����
			ImVec2(screenPos.x + displaySize + scaledOffsetX + Temp, screenPos.y + displaySize + scaledOffsetY),  // �� ��ġ���� ������ ����
			ImVec2(finalLeftTop.x, finalLeftTop.y),
			ImVec2(finalRightBottom.x, finalRightBottom.y)
		);
		draw_list->PopClipRect();

		// ����׿�: Ŭ���� ���� ǥ��
		draw_list->AddRect(clipMin, clipMax, IM_COL32(255, 0, 0, 255));

		RenderDrawLine();
	}


	ImGui::EndChild();
	ImGui::PopStyleColor();  // ��Ÿ�� ����

}

void AnimationUI::RenderAnimationPreview()
{
	// Child �������� ���� ����
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

			ImVec2 windowSize = ImGui::GetWindowSize();	// �߰�

			// ȭ�鿡 ǥ�õ� ũ�� ���
			float displaySize = 390.f;
			float scale = displaySize / currentSprite->GetBackground().x;

			// ��׶��� ���� ���
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

			// �����̽��� �̹����� ȭ�鿡�� ������ ���� ���
			float sliceWidth = currentSprite->GetSlice().x * scale;
			float sliceHeight = currentSprite->GetSlice().y * scale;

			// ��׶��� �߽��� (ȭ�� ����)
			float centerX = screenPos.x + (displaySize / 2.0f);
			float centerY = screenPos.y + (displaySize / 2.0f);

			// ������ ������ ����
			float scaledOffsetX = currentSprite->GetOffset().x * scale;
			float scaledOffsetY = currentSprite->GetOffset().y * scale;
			float Temp = -0.000976562500 * scale;

			// Ŭ���� ���� ��� (������ ����)
			ImVec2 clipMin(
				centerX - (sliceWidth / 2.0f) + scaledOffsetX + Temp,
				centerY - (sliceHeight / 2.0f) + scaledOffsetY
			);
			ImVec2 clipMax(
				centerX + (sliceWidth / 2.0f) + scaledOffsetX + Temp,
				centerY + (sliceHeight / 2.0f) + scaledOffsetY
			);

			// Ŭ���� ���� ����
			draw_list->PushClipRect(clipMin, clipMax, true);

			// �̹��� �׸��� - �̹��� ��ġ�� �����¸�ŭ �̵�
			draw_list->AddImage(
				SpTexture->GetSRV().Get(),
				ImVec2(screenPos.x + scaledOffsetX , screenPos.y + scaledOffsetY + Temp),
				ImVec2(screenPos.x + displaySize + scaledOffsetX + Temp, screenPos.y + displaySize + scaledOffsetY),
				ImVec2(finalLeftTop.x, finalLeftTop.y),
				ImVec2(finalRightBottom.x, finalRightBottom.y));

			draw_list->PopClipRect();

			// ����׿�: Ŭ���� ���� ǥ��
			draw_list->AddRect(clipMin, clipMax, IM_COL32(255, 0, 0, 255));

			RenderDrawLine();
			 
			if (Play)
				m_Time += EngineDT;
		}
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();  // ��Ÿ�� ����

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
	// �߽ɼ� �׸���
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	// ������
	ImVec2 lineStartY(windowPos.x + windowSize.x / 2, windowPos.y);
	ImVec2 lineEndY(windowPos.x + windowSize.x / 2, windowPos.y + windowSize.y);
	lineStartY.x += 1.f;
	lineEndY.x += 1.f;
	drawList->AddLine(lineStartY, lineEndY, IM_COL32(0, 125, 0, 125), 1.0f);

	float Line = 50.f;
	float Limit = 125.f;

	for (int i = 0; i < 4; ++i)
	{
		// ����
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}

	for (int i = 0; i < 4; ++i)
	{
		// ����
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 + (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit, windowPos.y + windowSize.y / 2 + (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}


	Line = 25.f;
	Limit = 150.f;

	for (int i = 1; i < 7; ++i)
	{
		// ����
		ImVec2 lineStartX(windowPos.x + Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		ImVec2 lineEndX((windowPos.x + windowSize.x) - Limit, windowPos.y + windowSize.y / 2 - (Line * i));
		drawList->AddLine(lineStartX, lineEndX, IM_COL32(0, 200, 0, 200), 1.0f);
	}

	for (int i = 1; i < 7; ++i)
	{
		// ����
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

	// ImGui�� IsItemHovered()�� ����Ͽ� �̹��� ���� ���콺�� �ִ��� Ȯ��
	bool isMouseInImage = ImGui::IsItemHovered();

	// �д� ���� - �̹��� ������ ��Ŭ���� ����
	if (!m_IsPanning && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isMouseInImage)
	{
		m_IsPanning = true;
		m_LastMousePos = m_MousePos;
	}
	// �д� �� - �巡�� ���� ��
	else if (m_IsPanning && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		float deltaX = m_MousePos.x - m_LastMousePos.x;
		float deltaY = m_MousePos.y - m_LastMousePos.y;

		m_PanOffset.x += deltaX;
		m_PanOffset.y += deltaY;

		m_LastMousePos = m_MousePos;
	}
	// �д� ����
	else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		m_IsPanning = false;
	}

	// ��Ŭ������ ��������Ʈ ���� - �̹��� �������� ����
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

			// 8���� �˻�
			for (int i = 0; i < 8; ++i)
			{
				int nx = x + dx[i];
				int ny = y + dy[i];

				if (nx < 0 || nx >= width || ny < 0 || ny >= height)
					continue;

				Vec2 nextPos((float)nx, (float)ny);
				if (m_PixelID.find(nextPos) == m_PixelID.end())
				{
					// ���İ��� �ִ� �ȼ��� �߰�
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