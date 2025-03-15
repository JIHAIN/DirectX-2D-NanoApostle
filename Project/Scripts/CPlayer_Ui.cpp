#include "pch.h"
#include "CPlayer_Ui.h"


CPlayer_Ui::CPlayer_Ui()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_UI)
{
}

CPlayer_Ui::~CPlayer_Ui()
{
}


void CPlayer_Ui::Begin()
{
}

void CPlayer_Ui::Tick()
{
}

void CPlayer_Ui::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Ui::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Ui::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Ui::SaveComponent(FILE* _File)
{
}

void CPlayer_Ui::LoadComponent(FILE* _File)
{
}
