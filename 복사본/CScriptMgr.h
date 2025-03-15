#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERASCRIPT,
	MISSILESCRIPT,
	PLAYERSCRIPT,
	PLAYER_ARROW_OBJ,
	PLAYER_ARROW_SCRIPT,
	PLAYER_ATK_OBJ,
	PLAYER_ATTACK_SCRIPT,
	PLAYER_HOOK_SCRIPT,
	SHIELD_SCRIPT,
	TURRETMISSILESCRIPT,
	TURRETSCRIPT,
	ALLSCRIPT,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
