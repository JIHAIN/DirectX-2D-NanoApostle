#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/CTaskMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CMissileScript.h"
#include "CPlayer_Arrow_Script.h"
#include "CPlayer_Ai.h"
#include "CPlayer_Move_Dust.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_PlayerSpeed(400)
	, Player_Move{ true ,true, true, true }
	, m_CurAnimState(Player_Ani_State::Idle_Down)
	, m_DustTime(0.f)
	, pPostProcess(nullptr)
	, pPostProcess1(nullptr)
	, pPostProcess2(nullptr)
{
	SetAI(true);

	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Player Speed", &m_PlayerSpeed });
	//AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "Missile", &m_Prefab });
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	SetAI(true);

	m_PlayerSpeed = 400.f;
	m_DustTime = 0.f;
	m_CurAnimState = Player_Ani_State::Idle_Down;
	pPostProcess = nullptr;
	pPostProcess1 = nullptr;
	pPostProcess2 = nullptr;

	memset(Player_Move, true, sizeof(Player_Move));

}

void CPlayerScript::AI_In()
{
	m_DustTime = 0.f;

	// 현재 눌린 키 확인
	bool isKeyA = KEY_PRESSED(KEY::A);
	bool isKeyD = KEY_PRESSED(KEY::D);
	bool isKeyW = KEY_PRESSED(KEY::W);
	bool isKeyS = KEY_PRESSED(KEY::S);

	// 눌린 키에 따라 적절한 애니메이션 재생
	if (isKeyA || isKeyD)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Move_Right, 10.f, true);
		m_CurAnimState = Player_Ani_State::Move_Right;
	}
	else if (isKeyW)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Move_Up, 10.f, true);
		m_CurAnimState = Player_Ani_State::Move_Up;
	}
	else if (isKeyS)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Move_Down, 10.f, true);
		m_CurAnimState = Player_Ani_State::Move_Down;
	}
	else  // 아무 키도 안눌려있으면 기존 방식대로
	{
		CPlayer_Ai* PAI = GETPAI();
		Vec3 PlayerDir = PAI->Get_Player_Dir();

		if (PlayerDir.x == -1.f || PlayerDir.x == 1.f)
		{
			FlipbookPlayer()->Play((UINT)Player_Ani_State::Idle_Right, 10.f, true);
			m_CurAnimState = Player_Ani_State::Idle_Right;
		}
		else if (PlayerDir.y == -1.f)
		{
			FlipbookPlayer()->Play((UINT)Player_Ani_State::Idle_Down, 10.f, true);
			m_CurAnimState = Player_Ani_State::Idle_Down;
		}
		else if (PlayerDir.y == 1.f)
		{
			FlipbookPlayer()->Play((UINT)Player_Ani_State::Idle_Up, 10.f, true);
			m_CurAnimState = Player_Ani_State::Idle_Up;
		}
	}
}

void CPlayerScript::AI_Tick()
{
	Vec3 PlayerCurPos = Transform()->GetRelativePos();
	bool isMoving = false;
	Player_Ani_State newState = m_CurAnimState;

	// 현재 눌린 키 상태 확인
	bool isKeyA = KEY_PRESSED(KEY::A);
	bool isKeyD = KEY_PRESSED(KEY::D);
	bool isKeyW = KEY_PRESSED(KEY::W);
	bool isKeyS = KEY_PRESSED(KEY::S);

	// 이동 처리 - 모든 방향 자유롭게
	if (isKeyA && Player_Move[0])
	{
		PlayerCurPos.x += -m_PlayerSpeed * DT;
		isMoving = true;
	}
	if (isKeyD && Player_Move[1])
	{
		PlayerCurPos.x += m_PlayerSpeed * DT;
		isMoving = true;
	}
	if (isKeyW && Player_Move[2])
	{
		PlayerCurPos.y += m_PlayerSpeed * DT;
		isMoving = true;
	}
	if (isKeyS && Player_Move[3])
	{
		PlayerCurPos.y -= m_PlayerSpeed * DT;
		isMoving = true;
	}

	// 애니메이션 우선순위 처리
	if (isKeyW || isKeyS)  // 수직 방향 우선
	{
		if (isKeyW)
			newState = Player_Ani_State::Move_Up;
		else if (isKeyS)
			newState = Player_Ani_State::Move_Down;
	}
	else if (isKeyA || isKeyD)  // 수평 방향
	{
		newState = Player_Ani_State::Move_Right;
	}
	else  // 키를 떼었을 때
	{
		switch (m_CurAnimState)
		{
		case Player_Ani_State::Move_Right:
			newState = Player_Ani_State::Idle_Right;
			break;
		case Player_Ani_State::Move_Up:
			newState = Player_Ani_State::Idle_Up;
			break;
		case Player_Ani_State::Move_Down:
			newState = Player_Ani_State::Idle_Down;
			break;
		default:
			break;
		}
	}

	// 애니메이션 상태가 변경될 때만 새로운 애니메이션 재생
	if (newState != m_CurAnimState)
	{
		if (FlipbookPlayer())
		{
			FlipbookPlayer()->Play((UINT)newState, 10.f, true);
			m_CurAnimState = newState;
		}
	}

	Transform()->SetRelativePos(PlayerCurPos);

	// 먼지 이펙트 처리
	if (isMoving)
	{
		m_DustTime += DT;
		if (m_DustTime >= 0.5f)
		{
			CreateDust();
			m_DustTime = 0.f;
		}
	}

	if (KEY_TAP(KEY::LBTN))
	{
		P_CHANGE(SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT1);
		return;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		P_CHANGE(SCRIPT_TYPE::PLAYER_DASH);
		return;
	}

	if (KEY_TAP(KEY::RBTN))
	{
		P_CHANGE(SCRIPT_TYPE::SHIELD_SCRIPT);
		return;
	}

	if (KEY_TAP(KEY::MBTN))
	{
		P_CHANGE(SCRIPT_TYPE::PLAYER_ARROW_SCRIPT);
		return;
	}

	// 포스트 프로세스 제어 연습코드
	PostProsed();

}


void CPlayerScript::AI_Exit()
{
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


void CPlayerScript::CreateDust()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();

	pPos.y -= 30.f;

	CGameObject* DustObj = new CGameObject;
	DustObj->AddComponent(new CMeshRender);
	DustObj->AddComponent(new CFlipbookPlayer);
	DustObj->AddComponent(new CPlayer_Move_Dust);
	DustObj->SetName(L"Dust");
	
	DustObj->Transform()->SetRelativePos(Vec3(pPos.x, pPos.y, pPos.z));
	DustObj->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 1.f));

	DustObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	DustObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	DustObj->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Dust"));
	DustObj->FlipbookPlayer()->Play(0, 15.f, false);

	CreateObject(DustObj, 4, false);
}

void CPlayerScript::PostProsed()
{
	static bool RoarOn = false;

	// 로어 효과
	if (KEY_TAP(KEY::NUM_2))
	{
		//FlipbookPlayer()->Play(0, 15.f, false);

		pPostProcess = new CGameObject;
		pPostProcess->SetName(L"PostRoar");
		pPostProcess->AddComponent(new CMeshRender);

		pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1.f));
		pPostProcess->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPostProcess->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RoarMtrl"));
		pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
		pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"pRoar"));

		CreateObject(pPostProcess, 4, false);

		RoarOn = true;
	}

	static float RoarTime = 0.f;

	if (RoarOn)
	{
		RoarTime += DT;

		if (RoarTime >= 2.5f)
		{
			CLevel* CLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			RoarTime = 0.f;


			tTask task = {};
			task.Type = TASK_TYPE::DELETE_OBJECT;
			task.Param0 = (DWORD_PTR)pPostProcess;
			task.Param1 = 4;

			CTaskMgr::GetInst()->AddTask(task);



			RoarOn = false;
		}
	}

	static bool ShockOn = false;

	// 쇼크웨이브 효과
	if (KEY_TAP(KEY::NUM_3))
	{
		pPostProcess1 = new CGameObject;
		pPostProcess1->SetName(L"ShockWave");
		pPostProcess1->AddComponent(new CMeshRender);

		pPostProcess1->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pPostProcess1->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPostProcess1->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShockWaveMtrl"));
		pPostProcess1->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));

		tTask task = {};
		task.Type = TASK_TYPE::CREATE_OBJECT;
		task.Param0 = (DWORD_PTR)pPostProcess1;
		task.Param1 = 4;

		CTaskMgr::GetInst()->AddTask(task);


		ShockOn = true;
	}

	static float ShockTime = 0.f;


	if (ShockOn)
	{
		ShockTime += DT;

		if (ShockTime >= 1.f)
		{
			CLevel* CLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			ShockTime = 0.f;

			tTask task = {};
			task.Type = TASK_TYPE::DELETE_OBJECT;
			task.Param0 = (DWORD_PTR)pPostProcess1;
			task.Param1 = 4;

			CTaskMgr::GetInst()->AddTask(task);

			ShockOn = false;


		}
	}


	static bool HitOn = false;

	// 쇼크웨이브 효과
	if (KEY_TAP(KEY::NUM_4))
	{
		//이거 맞았을때 이펙트
		pPostProcess2 = new CGameObject;
		pPostProcess2->SetName(L"HitEffect");
		pPostProcess2->AddComponent(new CMeshRender);

		pPostProcess2->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pPostProcess2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPostProcess2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HitEffectMtrl"));
		pPostProcess2->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));

		tTask task = {};
		task.Type = TASK_TYPE::CREATE_OBJECT;
		task.Param0 = (DWORD_PTR)pPostProcess2;
		task.Param1 = 4;

		CTaskMgr::GetInst()->AddTask(task);

		HitOn = true;
	}

	static float HitTime = 0.f;


	if (HitOn)
	{
		HitTime += DT;
		pPostProcess2->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_3, HitTime);

		if (HitTime >= 0.5f)
		{
			CLevel* CLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			HitTime = 0.f;

			tTask task = {};
			task.Type = TASK_TYPE::DELETE_OBJECT;
			task.Param0 = (DWORD_PTR)pPostProcess2;
			task.Param1 = 4;

			CTaskMgr::GetInst()->AddTask(task);

			HitOn = false;
		}
	}
}

void CPlayerScript::SaveComponent(FILE* _File)
{
	fwrite(&m_PlayerSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadComponent(FILE* _File)
{
	fread(&m_PlayerSpeed, sizeof(float), 1, _File);
}

