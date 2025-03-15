#include "pch.h"
#include "CPlayer_Move_Dust.h"

CPlayer_Move_Dust::CPlayer_Move_Dust()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_MOVE_DUST)
{
}

CPlayer_Move_Dust::~CPlayer_Move_Dust()
{
}


void CPlayer_Move_Dust::Begin()
{
}

void CPlayer_Move_Dust::Tick()
{
	if (GetOwner()->FlipbookPlayer()->isFinish())
	{
		DestroyObject(GetOwner());
	}
}

void CPlayer_Move_Dust::SaveComponent(FILE* _File)
{
}

void CPlayer_Move_Dust::LoadComponent(FILE* _File)
{
}
