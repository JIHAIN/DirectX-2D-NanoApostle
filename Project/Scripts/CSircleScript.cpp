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
	if (GetOwner()->GetName() == L"SircleTrap_1") // ��ó�� �ΰ��� �����Ǿ���ϴ°�
		m_Type = 1;
	else if (GetOwner()->GetName() == L"SircleTrap_2") // �� ��� �����ܾ��ϴ°�
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
			CreateAtk(Vec3(8000.f, playerY + 300.f, 21.f));      // ��
			CreateAtk(Vec3(8000.f, playerY, 21.f));              // �߰�
			CreateAtk(Vec3(8000.f, playerY - 300.f, 21.f));      // �Ʒ�
			
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
			CreateAtk(Vec3(388.f, 1500.f, 21.f));   // ù ��°
			CreateAtk(Vec3(1050.f, 1500.f, 21.f));  // �� ��°
			CreateAtk(Vec3(2625.f, 1500.f, 20.f));  // �� ��°
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

	// CAssetMgr �� ������ �����Ų ������ ����
	SircleObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare25.png"));
	SircleObj->MeshRender()->GetMaterial()->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Circle23.png"));
	SircleObj->Collider2D()->SetIndependentScale(true);
	SircleObj->Collider2D()->SetScale(Vec2(220.f, 220.f));

	CSircleObj* objScript = (CSircleObj*)SircleObj->GetScript((UINT)SCRIPT_TYPE::SIRCLEOBJ);
	objScript->SetType(m_Type);

	if (m_Type == 2)
	{
		// 0, 1, 2 ������ �ε��� ���� (1�� �߰�)
		static int index = 2;
		objScript->SetGroupIndex(index);
		index--;
	}

	if (m_Type == 3)
	{
		// Y��ǥ�� ��/�Ʒ� �׷� �Ǵ��ϰ� �׷� �ε��� ����
		int groupIndex = (_Pos.x - 16275.f) / 300.f;  // ù ��ġ �������� �ε��� ���
		objScript->SetGroupIndex(groupIndex);
	}

	CreateObject(SircleObj, 6, false);
}


