#include "pch.h"
#include "CPlayer_Hook_Script.h"
#include "CPlayer_Ai.h"

CPlayer_Hook_Script::CPlayer_Hook_Script()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_HOOK_SCRIPT)
	, AtkDmg(100)
	, m_Monster(nullptr)
	, m_Chain(nullptr)
	, m_MyPos(0.f,0.f,0.f)
	, m_fDashDuration(0.f)
	, m_fCurrentTime(0.f)
{
	SetAI(true);
}
	
CPlayer_Hook_Script::~CPlayer_Hook_Script()
{
}

void CPlayer_Hook_Script::Begin()
{
	SetAI(true);
	m_Chain = nullptr;
	m_Monster = nullptr;
	AtkDmg = 100;
	m_MyPos = Vec3(0.f, 0.f, 0.f);
	m_fDashDuration = 0.f;
	m_fCurrentTime = 0.f;
}

void CPlayer_Hook_Script::AI_In()
{
	m_fDashDuration = 0.3f;  // 1초로 설정
	m_fCurrentTime = 0.f;

	CPlayer_Ai* Ai = (CPlayer_Ai*)GetOwner()->GetScript(SCRIPT_TYPE::PLAYER_AI);
	m_Monster = Ai->GetGroggyMonster();

	m_MyPos = Transform()->GetWorldPos();
	Vec3 PlayerDir = m_Monster->Transform()->GetWorldPos() - m_MyPos;

	PlayerAniPlay1(PlayerDir.Normalize());
	// 몬스터 방향으로의 벡터 계산
	Vec3 DirectionToMonster = m_Monster->Transform()->GetWorldPos() - m_MyPos;
	float distance = DirectionToMonster.Length();  // 실제 거리
	Vec3 normalizedDir = DirectionToMonster / distance;  // 정규화된 방향

	// 체인 개수 계산 (20.f는 체인 하나의 길이)
	int chainCount = (int)(distance / 100.f);

	// 체인의 총 길이
	float chainLength = distance;

	// 회전 각도 계산 (라디안)
	float rotationZ = atan2(DirectionToMonster.y, DirectionToMonster.x);

	// 체인의 시작 위치 계산 (플레이어 위치에서 체인 길이의 절반만큼 몬스터 방향으로)
	Vec3 chainStartPos = m_MyPos + (normalizedDir * (chainLength * 0.5f));

	m_Chain = new CGameObject;
	m_Chain->AddComponent(new CRepeatSprite);
	m_Chain->SetName(L"Chain");

	// 위치, 회전, 스케일 설정
	m_Chain->Transform()->SetRelativePos(chainStartPos);
	m_Chain->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, rotationZ));
	m_Chain->RepeatSprite()->SetTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Chain.png"));
	m_Chain->RepeatSprite()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, 0.3f);
	m_Chain->RepeatSprite()->SetRepeatCount(chainCount, 1);
	m_Chain->RepeatSprite()->SetSingleImageSize(100.f, 20.f);  // 체인 한 개의 크기

	CreateObject(m_Chain, 0, false);

}

//void CPlayer_Hook_Script::AI_Tick() // 기존꺼
//{
//	m_fCurrentTime += DT;
//	float t = m_fCurrentTime / m_fDashDuration;  // 0~1 사이의 진행률
//
//	if (t >= 1.f)
//	{
//		Transform()->SetRelativePos(m_Monster->Transform()->GetRelativePos());
//
//		P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
//		return;
//	}
//	else
//	{
//		// 선형 보간으로 위치 계산
//		Vec3 newPos = Vec3::Lerp(m_MyPos, m_Monster->Transform()->GetRelativePos(), t);
//		Transform()->SetRelativePos(newPos);
//	}
//}

void CPlayer_Hook_Script::AI_Tick()
{
	m_fCurrentTime += DT;
	float t = m_fCurrentTime / m_fDashDuration;  // 0~1 사이의 진행률

	if (t >= 1.f)
	{
		Transform()->SetRelativePos(m_Monster->Transform()->GetRelativePos());
		P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
		return;
	}
	else
	{
		// 플레이어 위치 업데이트
		Vec3 newPos = Vec3::Lerp(m_MyPos, m_Monster->Transform()->GetRelativePos(), t);
		Transform()->SetRelativePos(newPos);

		// 체인 업데이트
		if (m_Chain)
		{
			// 새로운 거리 계산
			Vec3 currentDirection = m_Monster->Transform()->GetWorldPos() - newPos;
			float currentDistance = currentDirection.Length();
			Vec3 normalizedDir = currentDirection / currentDistance;

			// 체인 위치와 회전 업데이트
			Vec3 chainPos = newPos + (normalizedDir * (currentDistance * 0.5f));
			float rotationZ = atan2(currentDirection.y, currentDirection.x);

			m_Chain->Transform()->SetRelativePos(chainPos);
			m_Chain->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, rotationZ));

			// 체인 반복 횟수 업데이트
			int newChainCount = (int)(currentDistance / 100.f);
			m_Chain->RepeatSprite()->SetRepeatCount(newChainCount, 1);
		}
	}
}

void CPlayer_Hook_Script::AI_Exit()
{
	// 널 포인터로 막아버림
	m_Monster = nullptr;
	//CPlayer_Ai* Ai = (CPlayer_Ai*)GetOwner()->GetScript(SCRIPT_TYPE::PLAYER_AI);
	//Ai->Set_Groggy_Monster(m_Monster);

	DestroyObject(m_Chain);
	m_Chain = nullptr;
}

void CPlayer_Hook_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Hook_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Hook_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Hook_Script::SaveComponent(FILE* _File)
{
	fwrite(&AtkDmg, sizeof(int), 1, _File);
}

void CPlayer_Hook_Script::LoadComponent(FILE* _File)
{
	fread(&AtkDmg, sizeof(int), 1, _File);
}

void CPlayer_Hook_Script::PlayerAniPlay1(Vec3 _Dir)
{
	float angle = atan2(_Dir.x, _Dir.y) * 180.0f / XM_PI;

	// 방향 판정
	if (angle >= -45 && angle < 45)// 위쪽 (Up)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Up1, 10.f, false);
	}
	else if (angle >= 45 && angle < 135)// 오른쪽 (Right)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right1, 10.f, false);
	}
	else if (angle >= -135 && angle < -45) // 왼쪽 (Left)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right1, 10.f, false);
	}
	else // 아래쪽 (Down)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Down1, 10.f, false);
	}
}
