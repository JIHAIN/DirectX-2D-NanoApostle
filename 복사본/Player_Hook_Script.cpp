#include "pch.h"
#include "Player_Hook_Script.h"

Player_Hook_Script::Player_Hook_Script()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_HOOK_SCRIPT)

{
}

Player_Hook_Script::~Player_Hook_Script()
{
}
void Player_Hook_Script::Begin()
{
}

void Player_Hook_Script::Tick()
{
}

void Player_Hook_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Hook_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Hook_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

