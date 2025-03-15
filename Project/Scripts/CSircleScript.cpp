#include "pch.h"
#include "CSircleScript.h"
#include "CSircleObj.h"

CSircleScript::CSircleScript()
	: CScript((UINT)SCRIPT_TYPE::SIRCLESCRIPT)
	, m_Type(1)
{
}

CSircleScript::~CSircleScript()
{
}


void CSircleScript::Begin()
{
	if (GetOwner()->GetName() == L"SircleTrap_1") // 맨처음 두개가 생성되어야하는거
		m_Type = 1;
	else if (GetOwner()->GetName() == L"SircleTrap_2") // 나 계속 따라댕겨야하는거
		m_Type = 2;
	else if (GetOwner()->GetName() == L"SircleTrap_3")
		m_Type = 3;
	else
		m_Type = 4;
}

void CSircleScript::Tick()
{

}

void CSircleScript::SaveComponent(FILE* _File)
{
}

void CSircleScript::LoadComponent(FILE* _File)
{
}

void CSircleScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		if (m_Type == 1)
		{
			CreateAtk(Vec3(1812.f, 154.f, 1.f));
			CreateAtk(Vec3(2338, -416, 1.f));
		}
		else if (m_Type == 2)
		{
			
			float playerY = _OtherObject->Transform()->GetWorldPos().y;
			CreateAtk(Vec3(8000.f, playerY + 300.f, 21.f));      // 위
			CreateAtk(Vec3(8000.f, playerY, 21.f));              // 중간
			CreateAtk(Vec3(8000.f, playerY - 300.f, 21.f));      // 아래
			
		}
		else if (m_Type == 3)
		{
			CreateAtk(Vec3(16275.f, -365.f, 21.f));
			CreateAtk(Vec3(16575.f, -365.f, 21.f));
			CreateAtk(Vec3(16575.f+300.f, -365.f, 21.f));
			CreateAtk(Vec3(16575.f+600.f, -365.f, 21.f));
			CreateAtk(Vec3(16575.f + 900.f, -365.f, 21.f));
			CreateAtk(Vec3(16275.f, 260.f, 20.f));
			CreateAtk(Vec3(16575.f, 260.f, 20.f));
			CreateAtk(Vec3(16575.f + 300.f, 260.f, 20.f));
			CreateAtk(Vec3(16575.f + 600.f, 260.f, 20.f));
			CreateAtk(Vec3(16575.f + 900.f, 260.f, 20.f));

		}
		else
		{
			CreateAtk(Vec3(388.f, 1500.f, 21.f));   // 첫 번째
			CreateAtk(Vec3(1050.f, 1500.f, 21.f));  // 두 번째
			CreateAtk(Vec3(2625.f, 1500.f, 20.f));  // 세 번째
		}

	

	}
}

void CSircleScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSircleScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		DestroyObject(GetOwner());
	}
}

void CSircleScript::CreateAtk(Vec3 _Pos)
{
	CGameObject* SircleObj = new CGameObject;

	SircleObj->AddComponent(new CMeshRender);
	SircleObj->AddComponent(new CCollider2D); 
	SircleObj->AddComponent(new CSircleObj);
	SircleObj->SetName(L"SircleObj");
	SircleObj->Transform()->SetRelativePos(_Pos);
	SircleObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));
	SircleObj->Transform()->SetIndependentScale(true);
	SircleObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SircleObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TrapSircleMtrl"));

	// CAssetMgr 의 재질을 복사시킨 재질을 참조
	SircleObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare25.png"));
	SircleObj->MeshRender()->GetMaterial()->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Circle23.png"));
	SircleObj->Collider2D()->SetIndependentScale(true);
	SircleObj->Collider2D()->SetScale(Vec2(220.f, 220.f));

	CSircleObj* objScript = (CSircleObj*)SircleObj->GetScript((UINT)SCRIPT_TYPE::SIRCLEOBJ);
	objScript->SetType(m_Type);

	if (m_Type == 2)
	{
		// 0, 1, 2 순서로 인덱스 설정 (1이 중간)
		static int index = 2;
		objScript->SetGroupIndex(index);
		index--;
	}

	if (m_Type == 3)
	{
		// Y좌표로 위/아래 그룹 판단하고 그룹 인덱스 설정
		int groupIndex = (_Pos.x - 16275.f) / 300.f;  // 첫 위치 기준으로 인덱스 계산
		objScript->SetGroupIndex(groupIndex);
	}

	CreateObject(SircleObj, 6, false);
}


