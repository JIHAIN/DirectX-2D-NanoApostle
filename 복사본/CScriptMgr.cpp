#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraScript.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"
#include "Player_Arrow_Obj.h"
#include "Player_Arrow_Script.h"
#include "Player_Atk_Obj.h"
#include "Player_Attack_Script.h"
#include "Player_Hook_Script.h"
#include "Shield_Script.h"
#include "TurretMissileScript.h"
#include "TurretScript.h"
#include "WallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"Player_Arrow_Obj");
	_vec.push_back(L"Player_Arrow_Script");
	_vec.push_back(L"Player_Atk_Obj");
	_vec.push_back(L"Player_Attack_Script");
	_vec.push_back(L"Player_Hook_Script");
	_vec.push_back(L"Shield_Script");
	_vec.push_back(L"TurretMissileScript");
	_vec.push_back(L"TurretScript");
	_vec.push_back(L"WallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"Player_Arrow_Obj" == _strScriptName)
		return new Player_Arrow_Obj;
	if (L"Player_Arrow_Script" == _strScriptName)
		return new Player_Arrow_Script;
	if (L"Player_Atk_Obj" == _strScriptName)
		return new Player_Atk_Obj;
	if (L"Player_Attack_Script" == _strScriptName)
		return new Player_Attack_Script;
	if (L"Player_Hook_Script" == _strScriptName)
		return new Player_Hook_Script;
	if (L"Shield_Script" == _strScriptName)
		return new Shield_Script;
	if (L"TurretMissileScript" == _strScriptName)
		return new TurretMissileScript;
	if (L"TurretScript" == _strScriptName)
		return new TurretScript;
	if (L"WallScript" == _strScriptName)
		return new WallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::LAYER_ARROW_OBJ:
		return new Player_Arrow_Obj;
		break;
	case (UINT)SCRIPT_TYPE::LAYER_ARROW_SCRIPT:
		return new Player_Arrow_Script;
		break;
	case (UINT)SCRIPT_TYPE::LAYER_ATK_OBJ:
		return new Player_Atk_Obj;
		break;
	case (UINT)SCRIPT_TYPE::LAYER_ATTACK_SCRIPT:
		return new Player_Attack_Script;
		break;
	case (UINT)SCRIPT_TYPE::LAYER_HOOK_SCRIPT:
		return new Player_Hook_Script;
		break;
	case (UINT)SCRIPT_TYPE::HIELD_SCRIPT:
		return new Shield_Script;
		break;
	case (UINT)SCRIPT_TYPE::URRETMISSILESCRIPT:
		return new TurretMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::URRETSCRIPT:
		return new TurretScript;
		break;
	case (UINT)SCRIPT_TYPE::ALLSCRIPT:
		return new WallScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::LAYER_ARROW_OBJ:
		return L"Player_Arrow_Obj";
		break;

	case SCRIPT_TYPE::LAYER_ARROW_SCRIPT:
		return L"Player_Arrow_Script";
		break;

	case SCRIPT_TYPE::LAYER_ATK_OBJ:
		return L"Player_Atk_Obj";
		break;

	case SCRIPT_TYPE::LAYER_ATTACK_SCRIPT:
		return L"Player_Attack_Script";
		break;

	case SCRIPT_TYPE::LAYER_HOOK_SCRIPT:
		return L"Player_Hook_Script";
		break;

	case SCRIPT_TYPE::HIELD_SCRIPT:
		return L"Shield_Script";
		break;

	case SCRIPT_TYPE::URRETMISSILESCRIPT:
		return L"TurretMissileScript";
		break;

	case SCRIPT_TYPE::URRETSCRIPT:
		return L"TurretScript";
		break;

	case SCRIPT_TYPE::ALLSCRIPT:
		return L"WallScript";
		break;

	}
	return nullptr;
}