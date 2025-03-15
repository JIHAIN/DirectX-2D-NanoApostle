#include "pch.h"
#include "CAssetMgr.h"

CAssetMgr::CAssetMgr()
	: m_bAssetChanged(false)
{
}

CAssetMgr::~CAssetMgr()
{
}



Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);

	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Width, _Height, _PixelFormat, _BindFlag, _Usage)))
	{
		assert(nullptr);
		return nullptr;
	}

	pTex->SetKey(_Key);
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_Key, pTex.Get()));
	m_bAssetChanged = true;
	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);

	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
		return nullptr;
	}

	pTex->SetKey(_Key);
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_Key, pTex.Get()));
	m_bAssetChanged = true;
	return pTex;
}

void CAssetMgr::GetAssetNames(ASSET_TYPE _Type, vector<wstring>& _vecAssetNames)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecAssetNames.push_back(pair.first);
	}
}



void CAssetMgr::LoadAni(const wstring _Name)
{
	// �⺻ ��ο��� �ִϸ��̼� �߰� 
	wstring Path = CPathMgr::GetInst()->GetContentPath();

	//wstring filePath = L"Animation\\";
	//Path += filePath;

	//// �Է��� �̸� ����
	//wstring strName = _Name;
	//wstring FilePath = Path + strName + L".anim";

	// �Է��� �̸� ����
	wstring strName = _Name;
	wstring FilePath = Path + strName;

	// ���� ����
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath.c_str(), L"r");

	if (!pFile)
	{
		// ���� ���⿡ ������ ���
		MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
		return; // �Լ� ����
	}

	wchar_t szRead[255] = {};
	wstring AniName;
	Ptr<CTexture> EditAtlas = nullptr; 	// ������ �ؽ��� �̸� ����
	Ptr<CFlipbook> m_Flipbook = new CFlipbook;

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
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<CAsset>>& mapAsset = m_mapAsset[(UINT)_Type];

	map<wstring, Ptr<CAsset>>::iterator iter = mapAsset.find(_Key);

	if (mapAsset.end() == iter)
		return;

	mapAsset.erase(iter);

	m_bAssetChanged = true;
}
