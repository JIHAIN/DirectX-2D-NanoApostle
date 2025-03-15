#include "pch.h"
#include "CSpikScript.h"

#include "CSpikObjScript.h"

CSpikScript::CSpikScript()
	: CScript((UINT)SCRIPT_TYPE::SPIKSCRIPT)
	, m_Time(0.f)
{
}

CSpikScript::~CSpikScript()
{
}


void CSpikScript::Begin()
{
	m_Time = 0.f;

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Map\\Stage_GroundSpik.png"));

	Transform()->SetRelativeScale(130.f, 120.f, 1.f);
}


void CSpikScript::Tick()
{
	m_Time += DT;
}


void CSpikScript::SaveComponent(FILE* _File)
{
}

void CSpikScript::LoadComponent(FILE* _File)
{
}

void CSpikScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player" && m_Time >= 2.f)
	{
		m_Time = 0.f;
		CreateAtk();
	}
}

void CSpikScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpikScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpikScript::CreateAtk()
{
	Vec3 Pos = Transform()->GetRelativePos();
	Pos.z -= 1.f;

	CGameObject* m_SpikObj = new CGameObject;
	m_SpikObj->SetName(L"SpikAtk");
	m_SpikObj->AddComponent(new CMeshRender);
	m_SpikObj->AddComponent(new CCollider2D);
	m_SpikObj->AddComponent(new CSpikObjScript);

	m_SpikObj->Transform()->SetRelativePos(Pos);
	m_SpikObj->Transform()->SetRelativeScale(130.f, 120.f, 1.f);
	m_SpikObj->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_SpikObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_SpikObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpikEffectMtrl")); //SpikEffectMtrl
	m_SpikObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\FX_Flare.png"));
	m_SpikObj->MeshRender()->GetMaterial()->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Thunder.png"));

	CreateObject(m_SpikObj, 6, false);

}

