#include "pch.h"
#include "CWallOpen.h"
#include "CPlayer_Ai.h"
#include "CWallScript.h"

CWallOpen::CWallOpen()
	: CScript((UINT)SCRIPT_TYPE::WALLOPEN)
	, m_OpenTime(0.f)
	, m_ParentObj(nullptr)
	, m_ParentScript(nullptr)
{
}

CWallOpen::~CWallOpen()
{
}


void CWallOpen::Begin()
{
	m_OpenTime = 0.f;

	if (GetOwner()->GetParent())
	{
		m_ParentObj = GetOwner()->GetParent();
		m_ParentScript = (CWallScript*)m_ParentObj->GetScript(SCRIPT_TYPE::WALLSCRIPT);
	}
	
}

void CWallOpen::Tick()
{
	
	if (m_ParentScript->GetOpen())
	{
		m_OpenTime += DT;
		m_ParentObj->RepeatSprite()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_OpenTime);

		//m_Open = m_ParentScript->GetOpen();
	}
	else
	{
		m_OpenTime = 0.f;
		m_ParentObj->RepeatSprite()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_OpenTime);
	}
	
}

void CWallOpen::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

	if (!m_ParentScript->GetOpen() && _OtherObject->GetName() == L"pAtk1") // 비긴에서 플레이어 오브젝트랑 닿았으면 문을 열어줌
	{
		m_ParentScript->SetOpen(true); // 문열거라~ 하는거 
		//m_Open = m_ParentScript->GetOpen(); // 문열렸으니까 해야할거 해줌

		m_ParentObj->RepeatSprite()->GetDynamicMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\noise\\noise_03.jpg"));
	}
}

void CWallOpen::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CWallOpen::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

