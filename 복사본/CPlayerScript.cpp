#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/CTaskMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CMissileScript.h"


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_SCRIPT)
	, m_PlayerSpeed(500)
	, m_PaperBurnIntence(0.f)
	, m_PlayerCurPos(0.f, 0.f, 0.f)
	, m_PlayerPrevPos(0.f, 0.f, 0.f)
	, m_PlayerMoveDir(0.f,0.f,0.f)
	, m_PlayerDir(0.f, 0.f, 0.f)
	, Player_Move{ true ,true, true, true }
	, m_PlayDash(false)
	, m_state(CPlayer_Dash_State::Up)
	, pPostProcess(nullptr)
	, pPostProcess1(nullptr)
	, pPostProcess2(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}



void CPlayerScript::Begin()
{
	//파티클용 텍스트 그냥 여기서 해버림 나중에 지워
	//CAssetMgr::GetInst()->Load<CTexture>(L"PTex", L"Texture//particle//FX_Flare.png



}

void CPlayerScript::Tick()
{
	// 현재 위치 이전위치로 백업
	m_PlayerPrevPos = m_PlayerCurPos;

	m_PlayerCurPos = Transform()->GetRelativePos();

	Vec3 PlayerRotation = Transform()->GetRelativeRotation();
	static float time = 0.f;

	if (KEY_PRESSED(KEY::A) && Player_Move[0] && !m_PlayDash)
	{
		m_PlayerCurPos.x -= m_PlayerSpeed * DT;
		m_PlayerDir = Vec3(-1.f, 0.f, 0.f);
		m_state = CPlayer_Dash_State::Left;

		if (FlipbookPlayer())
		{
			PlayerRotation.y = XM_PI;
			FlipbookPlayer()->Play((int)Player_Ani_State::Move_Right, 10.f, true);
		}
	}
	else if (KEY_RELEASED(KEY::A) && Player_Move[0] && !m_PlayDash)
	{
		if (FlipbookPlayer())
		{
			PlayerRotation.y = XM_PI;
			FlipbookPlayer()->Play((int)Player_Ani_State::Idle_Right, 10.f, true);
		}
	}

	if (KEY_PRESSED(KEY::D) && Player_Move[1] && !m_PlayDash)
	{
		m_PlayerCurPos.x += m_PlayerSpeed * DT;
		m_PlayerDir = Vec3(1.f, 0.f, 0.f);
		m_state = CPlayer_Dash_State::Right;

		if (FlipbookPlayer())
		{
			PlayerRotation.y = 0;
			FlipbookPlayer()->Play((int)Player_Ani_State::Move_Right, 10.f, true);
		}
	}
	else if (KEY_RELEASED(KEY::D) && Player_Move[1] && !m_PlayDash)
	{
		if (FlipbookPlayer())
		{
			PlayerRotation.y = 0;
			FlipbookPlayer()->Play((int)Player_Ani_State::Idle_Right, 10.f, true);
		}
	}


	if (KEY_PRESSED(KEY::W) && Player_Move[2] && !m_PlayDash)
	{
		m_PlayerCurPos.y += m_PlayerSpeed * DT;
		m_PlayerDir = Vec3(0.f, 1.f, 0.f);
		m_state = CPlayer_Dash_State::Up;

		if (FlipbookPlayer())
		{
			FlipbookPlayer()->Play((int)Player_Ani_State::Move_Up, 10.f, true);
		}
	}
	else if (KEY_RELEASED(KEY::W) && Player_Move[2] && !m_PlayDash)
	{
		if (FlipbookPlayer())
		{
			FlipbookPlayer()->Play((int)Player_Ani_State::Idle_Up, 10.f, true);
		}
	}

	if (KEY_PRESSED(KEY::S) && Player_Move[3] && !m_PlayDash)
	{
		m_PlayerCurPos.y -= m_PlayerSpeed * DT;
		m_PlayerDir = Vec3(0.f, -1.f, 0.f);
		m_state = CPlayer_Dash_State::Down;

		if (FlipbookPlayer())
		{
			FlipbookPlayer()->Play((int)Player_Ani_State::Move_Down, 10.f, true);
		}
	}
	else if (KEY_RELEASED(KEY::S) && Player_Move[3] && !m_PlayDash)
	{
		if (FlipbookPlayer())
		{
			FlipbookPlayer()->Play((int)Player_Ani_State::Idle_Down, 10.f, true);
		}
	}

	// AW, AS DW DS 대각선 대쉬용
	{
		if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::W) && !m_PlayDash)
		{
			m_PlayerDir = Vec3(-1.f, 1.f, 0.f);
			m_state = CPlayer_Dash_State::LeftUP;
		}
		if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S) && !m_PlayDash)
		{
			m_PlayerDir = Vec3(-1.f, -1.f, 0.f);
			m_state = CPlayer_Dash_State::LeftDOWN;
		}
		if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W) && !m_PlayDash)
		{
			m_PlayerDir = Vec3(1.f, 1.f, 0.f);
			m_state = CPlayer_Dash_State::RightUP;
		}
		if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::S) && !m_PlayDash)
		{
			m_PlayerDir = Vec3(1.f, -1.f, 0.f);
			m_state = CPlayer_Dash_State::RightDOWN;
		}
	}
	if (KEY_TAP(KEY::SPACE))
	{
		m_PlayDash = true;
	}

	if (m_PlayDash) // 대쉬 상태일때
	{
		time += DT;

		if (time < 0.2f)
		{
			if (m_PlayerDir.x != 0.f && m_PlayerDir.y != 0.f) // 직선 대쉬일때랑 대각선 대쉬일때 거리의 차이를 줘야함
			{
				m_PlayerCurPos.x += m_PlayerDir.x * 1100 * DT;
				m_PlayerCurPos.y += m_PlayerDir.y * 1100 * DT;
			}
			else 
			{
				m_PlayerCurPos.x += m_PlayerDir.x * 1300 * DT;
				m_PlayerCurPos.y += m_PlayerDir.y * 1300 * DT;
			}
		}
		else
		{
			time = 0.f;
			m_PlayDash = false;
		}
	}
	else
		time = 0.f;

	//if (FlipbookPlayer())
	//{
	//	if (FlipbookPlayer()->isFinish())
	//	{
	//		FlipbookPlayer()->Play(1, 5.f, true);
	//	}
	//}
	

	if (KEY_PRESSED(KEY::NUM_0))
	{
		Vec3 vScale = Transform()->GetRelativeScale();
		vScale.x += DT * 0.1f;
		vScale.y += DT * 0.1f;
		Transform()->SetRelativeScale(vScale);
	}

	//if (KEY_TAP(KEY::Y))
	//{
	//	DrawDebugRect(Vec4(0.f, 1.f, 0.f, 0.5f), Transform()->GetRelativePos()
	//		, Vec2(300.f, 300.f), Vec3(0.f, 0.f, 0.f), true, 0.f);
	//}


	

	Transform()->SetRelativePos(m_PlayerCurPos);
	Transform()->SetRelativeRotation(PlayerRotation);

	// 이동한 방향알기
	m_PlayerMoveDir = m_PlayerCurPos - m_PlayerPrevPos;
	

	// 포스트 프로세스 제어 연습코드
	PostProsed();


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

void CPlayerScript::PostProsed()
{
	static bool RoarOn = false;

	// 로어 효과
	if (KEY_TAP(KEY::NUM_2))
	{
		FlipbookPlayer()->Play(0, 15.f, false);

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
		pPostProcess2 = new CGameObject;
		pPostProcess2->SetName(L"ShockWave");
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