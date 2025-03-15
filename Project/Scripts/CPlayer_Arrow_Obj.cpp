#include "pch.h"
#include "CPlayer_Arrow_Obj.h"

CPlayer_Arrow_Obj::CPlayer_Arrow_Obj(Vec3 _Velocity)
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ARROW_OBJ)
	, m_Speed(1500.f)
	, m_Velocity(_Velocity)
	, m_DeleteTime(0.f)
{
}

CPlayer_Arrow_Obj::CPlayer_Arrow_Obj()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ARROW_OBJ)
	, m_Speed(1500.f)
	, m_Velocity(0.f)
	, m_DeleteTime(0.f)
{
}

CPlayer_Arrow_Obj::~CPlayer_Arrow_Obj()
{
}

void CPlayer_Arrow_Obj::Begin()
{
}

void CPlayer_Arrow_Obj::Tick()
{
	m_DeleteTime += DT;

	if (m_DeleteTime >= 2.f)
	{
		DestroyObject(GetOwner());
		m_DeleteTime = 0;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	vPos += m_Velocity * m_Speed *DT;

	Transform()->SetRelativePos(vPos);


}

void CPlayer_Arrow_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Arrow_Obj::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Arrow_Obj::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Arrow_Obj::SaveComponent(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}

void CPlayer_Arrow_Obj::LoadComponent(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}


