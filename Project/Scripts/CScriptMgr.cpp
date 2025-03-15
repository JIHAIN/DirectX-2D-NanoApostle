#include "pch.h"
#include "CScriptMgr.h"

#include "CBoss_Ai.h"
#include "CBoss_Atk_Obj.h"
#include "CBoss_Bottom_Atk.h"
#include "CBoss_Charge_Atk.h"
#include "CBoss_Charge_Atk_Obj.h"
#include "CBoss_Dash.h"
#include "CBoss_Dead.h"
#include "CBoss_Groggy.h"
#include "CBoss_Idle.h"
#include "CBoss_Jump_Atk.h"
#include "CBoss_Jump_landing.h"
#include "CBoss_Nor_Atk1.h"
#include "CBoss_Nor_Atk2.h"
#include "CBoss_Nor_Atk3.h"
#include "CBoss_Roar.h"
#include "CCameraScript.h"
#include "CEB.h"
#include "CFaceMark_Script.h"
#include "CGroundBoundary.h"
#include "CLaserScript.h"
#include "CMap_BossMain.h"
#include "CMap_Main.h"
#include "CMissileScript.h"
#include "CMouseScript.h"
#include "CPlayerScript.h"
#include "CPlayer_Ai.h"
#include "CPlayer_Arrow_Obj.h"
#include "CPlayer_Arrow_Script.h"
#include "CPlayer_Atk_Obj.h"
#include "CPlayer_Attack_Script1.h"
#include "CPlayer_Attack_Script2.h"
#include "CPlayer_Attack_Script3.h"
#include "CPlayer_BossRoom.h"
#include "CPlayer_Dash.h"
#include "CPlayer_Death.h"
#include "CPlayer_End.h"
#include "CPlayer_Hook_Script.h"
#include "CPlayer_Main.h"
#include "CPlayer_Move_Dust.h"
#include "CPlayer_Ui.h"
#include "CShield_Script.h"
#include "CShield_Script_Obj.h"
#include "CSircleObj.h"
#include "CSircleScript.h"
#include "CSpikObjScript.h"
#include "CSpikScript.h"
#include "CTrap_Warp.h"
#include "CTurretMissileScript.h"
#include "CTurretScript.h"
#include "CUI_BossHP.h"
#include "CUI_Hpbar.h"
#include "CWallOpen.h"
#include "CWallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBoss_Ai");
	_vec.push_back(L"CBoss_Atk_Obj");
	_vec.push_back(L"CBoss_Bottom_Atk");
	_vec.push_back(L"CBoss_Charge_Atk");
	_vec.push_back(L"CBoss_Charge_Atk_Obj");
	_vec.push_back(L"CBoss_Dash");
	_vec.push_back(L"CBoss_Dead");
	_vec.push_back(L"CBoss_Groggy");
	_vec.push_back(L"CBoss_Idle");
	_vec.push_back(L"CBoss_Jump_Atk");
	_vec.push_back(L"CBoss_Jump_landing");
	_vec.push_back(L"CBoss_Nor_Atk1");
	_vec.push_back(L"CBoss_Nor_Atk2");
	_vec.push_back(L"CBoss_Nor_Atk3");
	_vec.push_back(L"CBoss_Roar");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CEB");
	_vec.push_back(L"CFaceMark_Script");
	_vec.push_back(L"CGroundBoundary");
	_vec.push_back(L"CLaserScript");
	_vec.push_back(L"CMap_BossMain");
	_vec.push_back(L"CMap_Main");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayer_Ai");
	_vec.push_back(L"CPlayer_Arrow_Obj");
	_vec.push_back(L"CPlayer_Arrow_Script");
	_vec.push_back(L"CPlayer_Atk_Obj");
	_vec.push_back(L"CPlayer_Attack_Script1");
	_vec.push_back(L"CPlayer_Attack_Script2");
	_vec.push_back(L"CPlayer_Attack_Script3");
	_vec.push_back(L"CPlayer_BossRoom");
	_vec.push_back(L"CPlayer_Dash");
	_vec.push_back(L"CPlayer_Death");
	_vec.push_back(L"CPlayer_End");
	_vec.push_back(L"CPlayer_Hook_Script");
	_vec.push_back(L"CPlayer_Main");
	_vec.push_back(L"CPlayer_Move_Dust");
	_vec.push_back(L"CPlayer_Ui");
	_vec.push_back(L"CShield_Script");
	_vec.push_back(L"CShield_Script_Obj");
	_vec.push_back(L"CSircleObj");
	_vec.push_back(L"CSircleScript");
	_vec.push_back(L"CSpikObjScript");
	_vec.push_back(L"CSpikScript");
	_vec.push_back(L"CTrap_Warp");
	_vec.push_back(L"CTurretMissileScript");
	_vec.push_back(L"CTurretScript");
	_vec.push_back(L"CUI_BossHP");
	_vec.push_back(L"CUI_Hpbar");
	_vec.push_back(L"CWallOpen");
	_vec.push_back(L"CWallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBoss_Ai" == _strScriptName)
		return new CBoss_Ai;
	if (L"CBoss_Atk_Obj" == _strScriptName)
		return new CBoss_Atk_Obj;
	if (L"CBoss_Bottom_Atk" == _strScriptName)
		return new CBoss_Bottom_Atk;
	if (L"CBoss_Charge_Atk" == _strScriptName)
		return new CBoss_Charge_Atk;
	if (L"CBoss_Charge_Atk_Obj" == _strScriptName)
		return new CBoss_Charge_Atk_Obj;
	if (L"CBoss_Dash" == _strScriptName)
		return new CBoss_Dash;
	if (L"CBoss_Dead" == _strScriptName)
		return new CBoss_Dead;
	if (L"CBoss_Groggy" == _strScriptName)
		return new CBoss_Groggy;
	if (L"CBoss_Idle" == _strScriptName)
		return new CBoss_Idle;
	if (L"CBoss_Jump_Atk" == _strScriptName)
		return new CBoss_Jump_Atk;
	if (L"CBoss_Jump_landing" == _strScriptName)
		return new CBoss_Jump_landing;
	if (L"CBoss_Nor_Atk1" == _strScriptName)
		return new CBoss_Nor_Atk1;
	if (L"CBoss_Nor_Atk2" == _strScriptName)
		return new CBoss_Nor_Atk2;
	if (L"CBoss_Nor_Atk3" == _strScriptName)
		return new CBoss_Nor_Atk3;
	if (L"CBoss_Roar" == _strScriptName)
		return new CBoss_Roar;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CEB" == _strScriptName)
		return new CEB;
	if (L"CFaceMark_Script" == _strScriptName)
		return new CFaceMark_Script;
	if (L"CGroundBoundary" == _strScriptName)
		return new CGroundBoundary;
	if (L"CLaserScript" == _strScriptName)
		return new CLaserScript;
	if (L"CMap_BossMain" == _strScriptName)
		return new CMap_BossMain;
	if (L"CMap_Main" == _strScriptName)
		return new CMap_Main;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayer_Ai" == _strScriptName)
		return new CPlayer_Ai;
	if (L"CPlayer_Arrow_Obj" == _strScriptName)
		return new CPlayer_Arrow_Obj;
	if (L"CPlayer_Arrow_Script" == _strScriptName)
		return new CPlayer_Arrow_Script;
	if (L"CPlayer_Atk_Obj" == _strScriptName)
		return new CPlayer_Atk_Obj;
	if (L"CPlayer_Attack_Script1" == _strScriptName)
		return new CPlayer_Attack_Script1;
	if (L"CPlayer_Attack_Script2" == _strScriptName)
		return new CPlayer_Attack_Script2;
	if (L"CPlayer_Attack_Script3" == _strScriptName)
		return new CPlayer_Attack_Script3;
	if (L"CPlayer_BossRoom" == _strScriptName)
		return new CPlayer_BossRoom;
	if (L"CPlayer_Dash" == _strScriptName)
		return new CPlayer_Dash;
	if (L"CPlayer_Death" == _strScriptName)
		return new CPlayer_Death;
	if (L"CPlayer_End" == _strScriptName)
		return new CPlayer_End;
	if (L"CPlayer_Hook_Script" == _strScriptName)
		return new CPlayer_Hook_Script;
	if (L"CPlayer_Main" == _strScriptName)
		return new CPlayer_Main;
	if (L"CPlayer_Move_Dust" == _strScriptName)
		return new CPlayer_Move_Dust;
	if (L"CPlayer_Ui" == _strScriptName)
		return new CPlayer_Ui;
	if (L"CShield_Script" == _strScriptName)
		return new CShield_Script;
	if (L"CShield_Script_Obj" == _strScriptName)
		return new CShield_Script_Obj;
	if (L"CSircleObj" == _strScriptName)
		return new CSircleObj;
	if (L"CSircleScript" == _strScriptName)
		return new CSircleScript;
	if (L"CSpikObjScript" == _strScriptName)
		return new CSpikObjScript;
	if (L"CSpikScript" == _strScriptName)
		return new CSpikScript;
	if (L"CTrap_Warp" == _strScriptName)
		return new CTrap_Warp;
	if (L"CTurretMissileScript" == _strScriptName)
		return new CTurretMissileScript;
	if (L"CTurretScript" == _strScriptName)
		return new CTurretScript;
	if (L"CUI_BossHP" == _strScriptName)
		return new CUI_BossHP;
	if (L"CUI_Hpbar" == _strScriptName)
		return new CUI_Hpbar;
	if (L"CWallOpen" == _strScriptName)
		return new CWallOpen;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BOSS_AI:
		return new CBoss_Ai;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_ATK_OBJ:
		return new CBoss_Atk_Obj;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_BOTTOM_ATK:
		return new CBoss_Bottom_Atk;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_CHARGE_ATK:
		return new CBoss_Charge_Atk;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_CHARGE_ATK_OBJ:
		return new CBoss_Charge_Atk_Obj;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_DASH:
		return new CBoss_Dash;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_DEAD:
		return new CBoss_Dead;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_GROGGY:
		return new CBoss_Groggy;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_IDLE:
		return new CBoss_Idle;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_JUMP_ATK:
		return new CBoss_Jump_Atk;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_JUMP_LANDING:
		return new CBoss_Jump_landing;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_NOR_ATK1:
		return new CBoss_Nor_Atk1;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_NOR_ATK2:
		return new CBoss_Nor_Atk2;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_NOR_ATK3:
		return new CBoss_Nor_Atk3;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_ROAR:
		return new CBoss_Roar;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::EB:
		return new CEB;
		break;
	case (UINT)SCRIPT_TYPE::FACEMARK_SCRIPT:
		return new CFaceMark_Script;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDBOUNDARY:
		return new CGroundBoundary;
		break;
	case (UINT)SCRIPT_TYPE::LASERSCRIPT:
		return new CLaserScript;
		break;
	case (UINT)SCRIPT_TYPE::MAP_BOSSMAIN:
		return new CMap_BossMain;
		break;
	case (UINT)SCRIPT_TYPE::MAP_MAIN:
		return new CMap_Main;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSESCRIPT:
		return new CMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_AI:
		return new CPlayer_Ai;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_ARROW_OBJ:
		return new CPlayer_Arrow_Obj;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_ARROW_SCRIPT:
		return new CPlayer_Arrow_Script;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_ATK_OBJ:
		return new CPlayer_Atk_Obj;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT1:
		return new CPlayer_Attack_Script1;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT2:
		return new CPlayer_Attack_Script2;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT3:
		return new CPlayer_Attack_Script3;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_BOSSROOM:
		return new CPlayer_BossRoom;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_DASH:
		return new CPlayer_Dash;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_DEATH:
		return new CPlayer_Death;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_END:
		return new CPlayer_End;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_HOOK_SCRIPT:
		return new CPlayer_Hook_Script;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_MAIN:
		return new CPlayer_Main;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_MOVE_DUST:
		return new CPlayer_Move_Dust;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_UI:
		return new CPlayer_Ui;
		break;
	case (UINT)SCRIPT_TYPE::SHIELD_SCRIPT:
		return new CShield_Script;
		break;
	case (UINT)SCRIPT_TYPE::SHIELD_SCRIPT_OBJ:
		return new CShield_Script_Obj;
		break;
	case (UINT)SCRIPT_TYPE::SIRCLEOBJ:
		return new CSircleObj;
		break;
	case (UINT)SCRIPT_TYPE::SIRCLESCRIPT:
		return new CSircleScript;
		break;
	case (UINT)SCRIPT_TYPE::SPIKOBJSCRIPT:
		return new CSpikObjScript;
		break;
	case (UINT)SCRIPT_TYPE::SPIKSCRIPT:
		return new CSpikScript;
		break;
	case (UINT)SCRIPT_TYPE::TRAP_WARP:
		return new CTrap_Warp;
		break;
	case (UINT)SCRIPT_TYPE::TURRETMISSILESCRIPT:
		return new CTurretMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::TURRETSCRIPT:
		return new CTurretScript;
		break;
	case (UINT)SCRIPT_TYPE::UI_BOSSHP:
		return new CUI_BossHP;
		break;
	case (UINT)SCRIPT_TYPE::UI_HPBAR:
		return new CUI_Hpbar;
		break;
	case (UINT)SCRIPT_TYPE::WALLOPEN:
		return new CWallOpen;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BOSS_AI:
		return L"CBoss_Ai";
		break;

	case SCRIPT_TYPE::BOSS_ATK_OBJ:
		return L"CBoss_Atk_Obj";
		break;

	case SCRIPT_TYPE::BOSS_BOTTOM_ATK:
		return L"CBoss_Bottom_Atk";
		break;

	case SCRIPT_TYPE::BOSS_CHARGE_ATK:
		return L"CBoss_Charge_Atk";
		break;

	case SCRIPT_TYPE::BOSS_CHARGE_ATK_OBJ:
		return L"CBoss_Charge_Atk_Obj";
		break;

	case SCRIPT_TYPE::BOSS_DASH:
		return L"CBoss_Dash";
		break;

	case SCRIPT_TYPE::BOSS_DEAD:
		return L"CBoss_Dead";
		break;

	case SCRIPT_TYPE::BOSS_GROGGY:
		return L"CBoss_Groggy";
		break;

	case SCRIPT_TYPE::BOSS_IDLE:
		return L"CBoss_Idle";
		break;

	case SCRIPT_TYPE::BOSS_JUMP_ATK:
		return L"CBoss_Jump_Atk";
		break;

	case SCRIPT_TYPE::BOSS_JUMP_LANDING:
		return L"CBoss_Jump_landing";
		break;

	case SCRIPT_TYPE::BOSS_NOR_ATK1:
		return L"CBoss_Nor_Atk1";
		break;

	case SCRIPT_TYPE::BOSS_NOR_ATK2:
		return L"CBoss_Nor_Atk2";
		break;

	case SCRIPT_TYPE::BOSS_NOR_ATK3:
		return L"CBoss_Nor_Atk3";
		break;

	case SCRIPT_TYPE::BOSS_ROAR:
		return L"CBoss_Roar";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::EB:
		return L"CEB";
		break;

	case SCRIPT_TYPE::FACEMARK_SCRIPT:
		return L"CFaceMark_Script";
		break;

	case SCRIPT_TYPE::GROUNDBOUNDARY:
		return L"CGroundBoundary";
		break;

	case SCRIPT_TYPE::LASERSCRIPT:
		return L"CLaserScript";
		break;

	case SCRIPT_TYPE::MAP_BOSSMAIN:
		return L"CMap_BossMain";
		break;

	case SCRIPT_TYPE::MAP_MAIN:
		return L"CMap_Main";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"CMouseScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYER_AI:
		return L"CPlayer_Ai";
		break;

	case SCRIPT_TYPE::PLAYER_ARROW_OBJ:
		return L"CPlayer_Arrow_Obj";
		break;

	case SCRIPT_TYPE::PLAYER_ARROW_SCRIPT:
		return L"CPlayer_Arrow_Script";
		break;

	case SCRIPT_TYPE::PLAYER_ATK_OBJ:
		return L"CPlayer_Atk_Obj";
		break;

	case SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT1:
		return L"CPlayer_Attack_Script1";
		break;

	case SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT2:
		return L"CPlayer_Attack_Script2";
		break;

	case SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT3:
		return L"CPlayer_Attack_Script3";
		break;

	case SCRIPT_TYPE::PLAYER_BOSSROOM:
		return L"CPlayer_BossRoom";
		break;

	case SCRIPT_TYPE::PLAYER_DASH:
		return L"CPlayer_Dash";
		break;

	case SCRIPT_TYPE::PLAYER_DEATH:
		return L"CPlayer_Death";
		break;

	case SCRIPT_TYPE::PLAYER_END:
		return L"CPlayer_End";
		break;

	case SCRIPT_TYPE::PLAYER_HOOK_SCRIPT:
		return L"CPlayer_Hook_Script";
		break;

	case SCRIPT_TYPE::PLAYER_MAIN:
		return L"CPlayer_Main";
		break;

	case SCRIPT_TYPE::PLAYER_MOVE_DUST:
		return L"CPlayer_Move_Dust";
		break;

	case SCRIPT_TYPE::PLAYER_UI:
		return L"CPlayer_Ui";
		break;

	case SCRIPT_TYPE::SHIELD_SCRIPT:
		return L"CShield_Script";
		break;

	case SCRIPT_TYPE::SHIELD_SCRIPT_OBJ:
		return L"CShield_Script_Obj";
		break;

	case SCRIPT_TYPE::SIRCLEOBJ:
		return L"CSircleObj";
		break;

	case SCRIPT_TYPE::SIRCLESCRIPT:
		return L"CSircleScript";
		break;

	case SCRIPT_TYPE::SPIKOBJSCRIPT:
		return L"CSpikObjScript";
		break;

	case SCRIPT_TYPE::SPIKSCRIPT:
		return L"CSpikScript";
		break;

	case SCRIPT_TYPE::TRAP_WARP:
		return L"CTrap_Warp";
		break;

	case SCRIPT_TYPE::TURRETMISSILESCRIPT:
		return L"CTurretMissileScript";
		break;

	case SCRIPT_TYPE::TURRETSCRIPT:
		return L"CTurretScript";
		break;

	case SCRIPT_TYPE::UI_BOSSHP:
		return L"CUI_BossHP";
		break;

	case SCRIPT_TYPE::UI_HPBAR:
		return L"CUI_Hpbar";
		break;

	case SCRIPT_TYPE::WALLOPEN:
		return L"CWallOpen";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	}
	return nullptr;
}