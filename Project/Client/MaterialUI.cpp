#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "ParamUI.h"

MaterialUI::MaterialUI()
	: AssetUI("Material", ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Render_Update()
{
	AssetTitle();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	string ShaderName;

	if (nullptr == pShader)
		ShaderName = "None";
	else
	{
		ShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
	}

	ImGui::Text("Shader");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MeshName", (char*)ShaderName.c_str(), ShaderName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::GRAPHIC_SHADER)
			{
				pMtrl->SetShader((CGraphicShader*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI 를 활성화 시키기
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("GraphicShader");
		pListUI->SetActive(true);

		// ListUI 에 넣어줄 문자열 정보 가져오기
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// 더블 클릭 시 호출시킬 함수 등록
		pListUI->AddDynamicDoubleCliked(this, (EUI_DELEGATE_2)&MaterialUI::SelectGraphicShader);
	}

	// 빈 메테리얼 이름 변환용 코드
	const string PREFIX = "Material\\";
	const string SUFFIX = ".mtrl";
	string MtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	size_t startPos = MtrlKey.find(PREFIX); // 앞부분 찾기
	size_t endPos = MtrlKey.rfind(SUFFIX);  // 뒷부분 찾기

	static char nameBuffer[256] = {};  // 수정할 이름을 담을 버퍼
	static string editedName;         // 버튼 클릭 시 저장될 수정된 이름

	if (MtrlKey.size() != 0 && startPos != string::npos && endPos != string::npos)
	{
		startPos += PREFIX.length();
		size_t nameLength = endPos - startPos;

		// 처음 포커스될 때만 현재 이름으로 버퍼 초기화
		if (ImGui::IsItemActivated())
		{
			string currentName = MtrlKey.substr(startPos, nameLength);
			strcpy_s(nameBuffer, currentName.c_str());
		}

		// 입력 처리
		if (ImGui::InputText("##KeySet123", nameBuffer, sizeof(nameBuffer)))
		{
			// 입력 시 변경사항은 버퍼에만 반영
		}
	}

	// 버튼 클릭 시 수정된 이름 저장
	if (ImGui::Button("##KeySet", ImVec2(30.f, 20.f)))
	{
		editedName = string(nameBuffer); // 버퍼의 내용을 저장
	}

	// Shader 에서 요청하는 파라미터 정보를 출력해준다.
	ShaderParameter();

	// 재질을 파일로 저장하기
	if (ImGui::Button("SAVE"))
	{
		Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
		assert(pMtrl.Get());

		string TempKey;
		TempKey = "Material\\" + editedName + ".mtrl";
		pMtrl->SetKey(wstring(TempKey.begin(), TempKey.end()));

		wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pMtrl->GetKey();
		wstring Test = pMtrl->GetKey();
		pMtrl->Save(strFilePath);
	}
	
}

void MaterialUI::SelectGraphicShader(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	if (nullptr == pMtrl)
		return;

	if (*pStr == "None")
	{
		pMtrl->SetShader(nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pShader)
		return;

	pMtrl->SetShader(pShader);
}

void MaterialUI::ShaderParameter()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	const vector<tScalarParam>& vecScalar = pShader->GetScalarParam();

	for (size_t i = 0; i < vecScalar.size(); ++i)
	{
		switch (vecScalar[i].eParam)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int* pData = (int*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			int Data = *pData;

			if (ParamUI::Param_Int(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float* pData = (float*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			float Data = *pData;

			if (ParamUI::Param_Float(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2* pData = (Vec2*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			Vec2 Data = *pData;

			if (ParamUI::Param_Vec2(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4* pData = (Vec4*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			Vec4 Data = *pData;

			if (ParamUI::Param_Vec4(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case MAT_0:
		case MAT_1:

			break;
		}
	}

	const vector<tTexParam>& vecTex = pShader->GetTexParam();
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTex[i].eParam);

		if (ParamUI::Param_Tex(vecTex[i].Desc, pTex
			, this, (EUI_DELEGATE_2)&MaterialUI::SelectTexture))
		{
			m_OpenTexType = vecTex[i].eParam;
			pMtrl->SetTexParam(vecTex[i].eParam, pTex);
		}
	}
}

void MaterialUI::SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	if (*pStr == "None")
	{
		pMtrl->SetTexParam(m_OpenTexType, nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	pMtrl->SetTexParam(m_OpenTexType, pTex);
}
