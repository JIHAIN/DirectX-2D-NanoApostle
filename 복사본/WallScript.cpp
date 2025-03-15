#include "pch.h"
#include "WallScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayerScript.h"

WallScript::WallScript()
	: CScript((UINT)SCRIPT_TYPE::WALL_SCRIPT)
	, m_Player(nullptr)
	, PrevPos(0.f, 0.f)
	, vOthObjPos(0.f, 0.f)
{
}

WallScript::~WallScript()
{
}

void WallScript::Begin()
{
	CLevel* CurLel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* PlayerLayer = CurLel->GetLayer(3);
	vector<CGameObject*> pPlayerVec = PlayerLayer->GetParentObjects();

	for (int i = 0; i < pPlayerVec.size(); ++i)
	{
		if (pPlayerVec[i]->GetName() == L"Player")
		{
			m_Player = pPlayerVec[i];
		}
	}

}

void WallScript::Tick()
{

}

void WallScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		Vec2 vMyColliderPos = Vec2(_Collider->GetColliderWorldMat().Translation().x, _Collider->GetColliderWorldMat().Translation().y);
		Vec2 vMyColliderSclae = Vec2(_Collider->Transform()->GetWorldScale().x, _Collider->Transform()->GetWorldScale().y);

		Vec2 vOtherColliderScale = Vec2(_OtherCollider->Transform()->GetWorldScale().x, _OtherCollider->Transform()->GetWorldScale().y);
		Vec2 vOtherColliderPos = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x, _OtherCollider->GetColliderWorldMat().Translation().y);
		Vec2 vOthObjPos = Vec2(_OtherObject->Transform()->GetWorldPos().x, _OtherObject->Transform()->GetWorldPos().y);
		float vOthObjPosZ = _OtherObject->Transform()->GetWorldPos().z;

		Vec2 vDiff = vMyColliderPos - vOtherColliderPos;

		{
			// 두 충돌체의 크기를 절반씩 합친 값
			float ScaleX = _Collider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
			float ScaleY = _Collider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

			// 충돌 깊이 계산
			float overlapX = ScaleX - vDiff.x;
			float overlapY = ScaleY - vDiff.y;
		}

		// 상대 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
		Vec2 OthMin = Vec2(vOthObjPos.x - vOtherColliderScale.x / 2.f, vOthObjPos.y + vOtherColliderScale.y / 2.f);
		Vec2 OthMax = Vec2(vOthObjPos.x + vOtherColliderScale.x / 2.f, vOthObjPos.y - vOtherColliderScale.y / 2.f);

		// 내 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
		Vec2 myMin = Vec2(vMyColliderPos.x - vMyColliderSclae.x / 2.f, vMyColliderPos.y + vMyColliderSclae.y / 2.f);
		Vec2 myMax = Vec2(vMyColliderPos.x + vMyColliderSclae.x / 2.f, vMyColliderPos.y - vMyColliderSclae.y / 2.f);

		vector<CScript*> PScript = _OtherObject->GetScripts(); // 플레이어 스크립트 가져오기
		CPlayerScript* playerScript = nullptr;
		for (int i = 0; i < PScript.size(); ++i)
		{
			playerScript = dynamic_cast<CPlayerScript*>(PScript[i]);
			if (playerScript != nullptr)
			{
				break;
			}
		}

		Vec3 PlayerDir = playerScript->GetPlayerDir();
		float pDirX = PlayerDir.x;
		float pDirY = PlayerDir.y;

		CPlayer_Dash_State PlayerDashState = playerScript->GetDashState();

		// 왼쪽에서 오른쪽으로
		if (myMin.x < OthMax.x && abs(myMin.x - OthMax.x) < 30.f && OthMax.x < myMax.x && OthMin.y > myMax.y && OthMax.y < myMin.y) // 상대가 왼쪽에서 
		{
			playerScript->Set_Player_Move_limit(1, false); // 오른쪽 이동금지

			// 대쉬중 이라면
			if (PlayerDashState == CPlayer_Dash_State::Right)
			{
				playerScript->SetDash(false);
			}
			else if (PlayerDashState == CPlayer_Dash_State::RightUP || PlayerDashState == CPlayer_Dash_State::RightDOWN)
			{
				PlayerDir.x = 0.f;
				playerScript->SetPlayerDir(PlayerDir);
			}
		}
		// 오른쪽에서 왼쪽으로
		else if (myMax.x > OthMin.x && abs(myMax.x - OthMin.x) < 30.f && OthMin.x > myMin.x && OthMin.y > myMax.y && OthMax.y < myMin.y)  // 상대가 오른쪽에서
		{
			playerScript->Set_Player_Move_limit(0, false); // 왼쪽 이동금지

			if (PlayerDashState == CPlayer_Dash_State::Left)
			{
				playerScript->SetDash(false);
			}
			else if (PlayerDashState == CPlayer_Dash_State::LeftUP || PlayerDashState == CPlayer_Dash_State::LeftDOWN)
			{
				PlayerDir.x = 0.f;
				playerScript->SetPlayerDir(PlayerDir);
			}
		}
		else if (myMax.y < OthMin.y && abs(myMax.y - OthMin.y) < 30.f && myMin.y > OthMin.y)  // 상대가 아래에서
		{
			playerScript->Set_Player_Move_limit(2, false); // 위쪽 이동금지

			if (PlayerDashState == CPlayer_Dash_State::Up)
			{
				playerScript->SetDash(false);
			}
			else if (PlayerDashState == CPlayer_Dash_State::LeftUP || PlayerDashState == CPlayer_Dash_State::RightUP)
			{
				PlayerDir.y = 0.f;
				playerScript->SetPlayerDir(PlayerDir);
			}
		}
		else if (myMin.y > OthMax.y && abs(myMin.y - OthMax.y) < 30.f && myMax.y < OthMax.y) // 상대가 위에서
		{
			playerScript->Set_Player_Move_limit(3, false); // 아래쪽 이동금지

			if (PlayerDashState == CPlayer_Dash_State::Down)
			{
				playerScript->SetDash(false);
			}
			else if (PlayerDashState == CPlayer_Dash_State::LeftDOWN || PlayerDashState == CPlayer_Dash_State::RightDOWN)
			{
				PlayerDir.y = 0.f;
				playerScript->SetPlayerDir(PlayerDir);
			}
		}
	}
}

void WallScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		PrevPos = Vec2(vOthObjPos.x, vOthObjPos.y);

		vector<CScript*> PScript = _OtherObject->GetScripts(); // 플레이어 스크립트 가져오기
		CPlayerScript* playerScript = nullptr;

		for (int i = 0; i < PScript.size(); ++i)
		{
			playerScript = dynamic_cast<CPlayerScript*>(PScript[i]);
			if (playerScript != nullptr)
			{
				break;
			}
		}

		Vec2 vMyColliderPos = Vec2(_Collider->GetColliderWorldMat().Translation().x, _Collider->GetColliderWorldMat().Translation().y);
		Vec2 vMyColliderSclae = Vec2(_Collider->Transform()->GetWorldScale().x, _Collider->Transform()->GetWorldScale().y);

		Vec2 vOtherColliderScale = Vec2(_OtherCollider->Transform()->GetWorldScale().x, _OtherCollider->Transform()->GetWorldScale().y);
		Vec2 vOtherColliderPos = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x, _OtherCollider->GetColliderWorldMat().Translation().y);
		vOthObjPos = Vec2(_OtherObject->Transform()->GetWorldPos().x, _OtherObject->Transform()->GetWorldPos().y);
		float vOthObjPosZ = _OtherObject->Transform()->GetWorldPos().z;

		Vec2 vDiff = vMyColliderPos - vOtherColliderPos;

		{
			// 두 충돌체의 크기를 절반씩 합친 값
			float ScaleX = _Collider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
			float ScaleY = _Collider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

			// 충돌 깊이 계산
			float overlapX = ScaleX - vDiff.x;
			float overlapY = ScaleY - vDiff.y;
		}

		// 상대 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
		Vec2 OthMin = Vec2(vOthObjPos.x - vOtherColliderScale.x / 2.f, vOthObjPos.y + vOtherColliderScale.y / 2.f);
		Vec2 OthMax = Vec2(vOthObjPos.x + vOtherColliderScale.x / 2.f, vOthObjPos.y - vOtherColliderScale.y / 2.f);

		// 내 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
		Vec2 myMin = Vec2(vMyColliderPos.x - vMyColliderSclae.x / 2.f, vMyColliderPos.y + vMyColliderSclae.y / 2.f);
		Vec2 myMax = Vec2(vMyColliderPos.x + vMyColliderSclae.x / 2.f, vMyColliderPos.y - vMyColliderSclae.y / 2.f);


		Vec3 PlayerDir = playerScript->GetPlayerDir();
		float pDirX = PlayerDir.x;
		float pDirY = PlayerDir.y;

		CPlayer_Dash_State PlayerDashState = playerScript->GetDashState();

		if (myMin.x < OthMax.x && abs(myMin.x - OthMax.x) < 30.f && OthMax.x < myMax.x && OthMin.y > myMax.y && OthMax.y < myMin.y) // 상대가 왼쪽에서 
		{
			playerScript->Set_Player_Move_limit(1, false); // 오른쪽 이동금지

			if (PlayerDir.x > 0.f) // x 가 양수로 가면 오른쪽으로 가는거니까 0으로 만들어버림
				PlayerDir.x = 0.f;
			playerScript->SetPlayerDir(PlayerDir);
		}
		else if (myMax.x > OthMin.x && abs(myMax.x - OthMin.x) < 30.f && OthMin.x > myMin.x && OthMin.y > myMax.y && OthMax.y < myMin.y)  // 상대가 오른쪽에서
		{
			playerScript->Set_Player_Move_limit(0, false); // 왼쪽 이동금지

			if (PlayerDir.x < 0.f)
				PlayerDir.x = 0.f;
			playerScript->SetPlayerDir(PlayerDir);
		}
		else if (myMax.y < OthMin.y && abs(myMax.y - OthMin.y) < 30.f && myMin.y > OthMin.y)  // 상대가 아래에서
		{
			playerScript->Set_Player_Move_limit(2, false); // 위쪽 이동금지

			if (PlayerDir.y > 0.f)
				PlayerDir.y = 0.f;
			playerScript->SetPlayerDir(PlayerDir);
		}
		else if (myMin.y > OthMax.y && abs(myMin.y - OthMax.y) < 30.f && myMax.y < OthMax.y) // 상대가 위에서
		{
			playerScript->Set_Player_Move_limit(3, false); // 아래쪽 이동금지

			if (PlayerDir.y < 0.f)
				PlayerDir.y = 0.f;
			playerScript->SetPlayerDir(PlayerDir);
		}


	}
}

void WallScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	vector<CScript*> PScript = _OtherObject->GetScripts(); // 플레이어 스크립트 가져오기
	CPlayerScript* playerScript = nullptr;

	for (int i = 0; i < PScript.size(); ++i)
	{
		playerScript = dynamic_cast<CPlayerScript*>(PScript[i]);
		if (playerScript != nullptr)
		{
			break;
		}
	}

	playerScript->Set_Player_Move_limit(0, true);
	playerScript->Set_Player_Move_limit(1, true);
	playerScript->Set_Player_Move_limit(2, true);
	playerScript->Set_Player_Move_limit(3, true);
}

