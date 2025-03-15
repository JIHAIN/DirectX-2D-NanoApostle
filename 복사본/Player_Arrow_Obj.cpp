#include "pch.h"
#include "Player_Arrow_Obj.h"

Player_Arrow_Obj::Player_Arrow_Obj(Vec3 _Velocity)
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ARROW_SCRIPT)
	, m_Speed(1500.f)
	, m_Velocity(_Velocity)
	, m_DeleteTime(0.f)
{
}

Player_Arrow_Obj::~Player_Arrow_Obj()
{
}

void Player_Arrow_Obj::Begin()
{
}

void Player_Arrow_Obj::Tick()
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

void Player_Arrow_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Arrow_Obj::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Arrow_Obj::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


