#include "pch.h"
#include "CPlayer_End.h"

CPlayer_End::CPlayer_End()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_END)
{
}

CPlayer_End::~CPlayer_End()
{
}

void CPlayer_End::Begin()
{
}

void CPlayer_End::SaveComponent(FILE* _File)
{
}

void CPlayer_End::LoadComponent(FILE* _File)
{
}

void CPlayer_End::AI_In()
{
	FlipbookPlayer()->Play((UINT)Player_Ani_State::EndBoss, 6.f, false);

}

void CPlayer_End::AI_Tick()
{
}

void CPlayer_End::AI_Exit()
{
}

void CPlayer_End::CreatePost()
{
}


