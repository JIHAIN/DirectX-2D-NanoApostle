#include "pch.h"
#include "CPlayer_Arrow_Script.h"
#include "CPlayer_Arrow_Obj.h"
#include "CPlayer_Ai.h"

CPlayer_Arrow_Script::CPlayer_Arrow_Script()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ARROW_SCRIPT)
{
}

CPlayer_Arrow_Script::~CPlayer_Arrow_Script()
{
}


void CPlayer_Arrow_Script::Begin()
{
	SetAI(true);
}

void CPlayer_Arrow_Script::Tick()
{
}

void CPlayer_Arrow_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Arrow_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Arrow_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Arrow_Script::SaveComponent(FILE* _File)
{
}

void CPlayer_Arrow_Script::LoadComponent(FILE* _File)
{

}

void CPlayer_Arrow_Script::AI_In()
{
	Vec3 MousePos = g_MouseInfo.MouseWorldPos;
	Vec3 pWPos = Transform()->GetWorldPos();
	Vec3 PlayerMouseDir = MousePos - pWPos;
	MousDir = PlayerMouseDir.Normalize();
	PlayerAniPlay1(MousDir);

	CreateArrow();
}

void CPlayer_Arrow_Script::AI_Tick()
{
	if (KEY_PRESSED(KEY::MBTN))
	{

	}
	
	// 애니메이션이 끝나거나 시간이 초과되면 기본 상태로
	if (FlipbookPlayer()->isFinish())
	{
		P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
		return;
	}
}

void CPlayer_Arrow_Script::AI_Exit()
{
}

void CPlayer_Arrow_Script::PlayerAniPlay1(Vec3 _Dir)
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


void CPlayer_Arrow_Script::CreateArrow()
{
	Vec3 MousePos = g_MouseInfo.MouseWorldPos;

	Vec3 pWPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 PlayerMouseDir = MousePos - pWPos;
	PlayerMouseDir.Normalize();

	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();

	CGameObject* Arrow = new CGameObject;
	Arrow->AddComponent(new CMeshRender);
	Arrow->AddComponent(new CCollider2D);
	Arrow->AddComponent(new CFlipbookPlayer);
	// 기본 생성자 없애둬서 Vec3 타입의 방향벡터 넣어줘야함
	Arrow->AddComponent(new CPlayer_Arrow_Obj(PlayerMouseDir));
	Arrow->SetName(L"Arrow");
	Arrow->Transform()->SetRelativePos(pPos);
	Arrow->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
	Arrow->Transform()->SetIndependentScale(true);
	Arrow->Transform()->SetRelativeRotation(PlayerMouseDir);
	Arrow->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Arrow->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	//Arrow->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
	//Arrow->FlipbookPlayer()->Play(0, 100.f, false);
	Arrow->Collider2D()->SetIndependentScale(false);
	Arrow->Collider2D()->SetScale(Vec2(1.f, 1.f));
	CreateObject(Arrow, 4, false);
}

