#include "pch.h"
#include "CSpikObjScript.h"

CSpikObjScript::CSpikObjScript()
	: CScript((UINT)SCRIPT_TYPE::SPIKOBJSCRIPT)
	, m_Time(0.f)
{
}

CSpikObjScript::~CSpikObjScript()
{
}


void CSpikObjScript::Begin()
{
	m_Time = 0.f;
}

void CSpikObjScript::Tick()
{
	m_Time += DT;

	if (m_Time >= 2.f)
	{
		DestroyObject(GetOwner());
	}
}



void CSpikObjScript::SaveComponent(FILE* _File)
{
}

void CSpikObjScript::LoadComponent(FILE* _File)
{
}

void CSpikObjScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpikObjScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpikObjScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}




