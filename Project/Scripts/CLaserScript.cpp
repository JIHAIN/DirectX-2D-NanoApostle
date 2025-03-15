#include "pch.h"
#include "CLaserScript.h"

CLaserScript::CLaserScript()
	: CScript((UINT)SCRIPT_TYPE::LASERSCRIPT)
{
}

CLaserScript::~CLaserScript()
{
}

void CLaserScript::Begin()
{
}

void CLaserScript::SaveComponent(FILE* _File)
{
}

void CLaserScript::LoadComponent(FILE* _File)
{
}

void CLaserScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLaserScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLaserScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLaserScript::CreateAtk()
{
}

void CLaserScript::AI_Tick()
{
}

void CLaserScript::AI_In()
{
}

void CLaserScript::AI_Exit()
{
}


