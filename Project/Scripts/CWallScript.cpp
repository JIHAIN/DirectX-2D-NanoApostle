#include "pch.h"
#include "CWallScript.h"

#include "CPlayer_Ai.h"
#include "CPlayerScript.h"

CWallScript::CWallScript()
	: CScript((UINT)SCRIPT_TYPE::WALLSCRIPT)
	, Player_Move_Lock{true, true, true, true}
	, m_Open(false)
{
}

CWallScript::~CWallScript()
{
}

void CWallScript::Begin()
{
	m_Open = false;
	memset(Player_Move_Lock, true, sizeof(Player_Move_Lock));
}

void CWallScript::Tick()
{

}

void CWallScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() != L"Player" || m_Open)
		return;

	CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	// 벽과 플레이어의 중심점
	Vec2 wallCenter = Vec2(_Collider->GetColliderWorldMat().Translation().x,
		_Collider->GetColliderWorldMat().Translation().y);
	Vec2 playerCenter = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x,
		_OtherCollider->GetColliderWorldMat().Translation().y);

	// z축 회전 확인 (90도에 가까운지)
	float rotationZ = Transform()->GetRelativeRotation().z;
	bool isRotated90 = abs(abs(rotationZ) - XM_PI / 2) < 0.1f;  // 90도 회전 여부 체크

	// 충돌체의 크기
	Vec2 wallScale;
	if (isRotated90)
	{
		// 90도 회전된 경우 x,y 스왑
		wallScale = Vec2(_Collider->Transform()->GetWorldScale().y,
			_Collider->Transform()->GetWorldScale().x);
	}
	else
	{
		// 일반적인 경우
		wallScale = Vec2(_Collider->Transform()->GetWorldScale().x,
			_Collider->Transform()->GetWorldScale().y);
	}

	Vec2 playerScale = Vec2(_OtherCollider->Transform()->GetWorldScale().x,
		_OtherCollider->Transform()->GetWorldScale().y);

	// 거리 계산
	Vec2 distance = playerCenter - wallCenter;

	// 각 방향의 겹침
	float overlapX = (wallScale.x + playerScale.x) * 0.5f - abs(distance.x);
	float overlapY = (wallScale.y + playerScale.y) * 0.5f - abs(distance.y);

	if (overlapX < overlapY)
	{
		// X축 방향 충돌
		if (distance.x > 0)
		{
			// 플레이어가 오른쪽에 있음
			playerScript->Set_Player_Move_limit(0, false);
			Player_Move_Lock[0] = false;
		}
		else
		{
			// 플레이어가 왼쪽에 있음
			playerScript->Set_Player_Move_limit(1, false);
			Player_Move_Lock[1] = false;
		}
	}
	else
	{
		// Y축 방향 충돌
		if (distance.y > 0)
		{
			// 플레이어가 위에 있음
			playerScript->Set_Player_Move_limit(3, false);
			Player_Move_Lock[3] = false;
		}
		else
		{
			// 플레이어가 아래에 있음
			playerScript->Set_Player_Move_limit(2, false);
			Player_Move_Lock[2] = false;
		}
	}
}

void CWallScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

		for (int i = 0; i < 4; ++i)
		{
			if (Player_Move_Lock[i] == false)
			{
				playerScript->Set_Player_Move_limit(i, true);
			}
		}

		// 딱 잠군거만 풀어줌 

		memset(Player_Move_Lock, true, sizeof(Player_Move_Lock));// 다시 트루로 세팅

		if (m_Open)
		{
			m_Open = false;
		}
	}

}

void CWallScript::SaveComponent(FILE* _File)
{
}

void CWallScript::LoadComponent(FILE* _File)
{
}

