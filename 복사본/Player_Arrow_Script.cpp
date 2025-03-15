#include "pch.h"
#include "Player_Arrow_Script.h"
#include "Player_Arrow_Obj.h"

Player_Arrow_Script::Player_Arrow_Script()
{
}

Player_Arrow_Script::~Player_Arrow_Script()
{
}


void Player_Arrow_Script::Begin()
{
}

void Player_Arrow_Script::Tick()
{

	if (KEY_TAP(KEY::MBTN))
	{
		CreateArrow();
	}

	if (KEY_PRESSED(KEY::LSHIFT))
	{

	}
}

void Player_Arrow_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Arrow_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Arrow_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Arrow_Script::CreateArrow()
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
	Arrow->AddComponent(new Player_Arrow_Obj(PlayerMouseDir));
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

