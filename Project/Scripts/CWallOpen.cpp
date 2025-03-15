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

	if (!m_ParentScript->GetOpen() && _OtherObject->GetName() == L"pAtk1") // ��信�� �÷��̾� ������Ʈ�� ������� ���� ������
	{
		m_ParentScript->SetOpen(true); // �����Ŷ�~ �ϴ°� 
		//m_Open = m_ParentScript->GetOpen(); // ���������ϱ� �ؾ��Ұ� ����

		m_ParentObj->RepeatSprite()->GetDynamicMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\noise\\noise_03.jpg"));
	}
}

void CWallOpen::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CWallOpen::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

