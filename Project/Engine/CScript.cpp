#include "pch.h"
#include "CScript.h"



CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
	, m_AI(false)
{
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _Pref, Vec3 _WorldPos, int _Layer)
{
	if (nullptr == _Pref)
		return;

	CGameObject* pNewObject = _Pref->Instantiate();

	pNewObject->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pNewObject, _Layer, false);
}