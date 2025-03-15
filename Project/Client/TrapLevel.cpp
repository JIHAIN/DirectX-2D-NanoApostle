#include "pch.h"
#include "TrapLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>
#include <Engine/enum.h>

#include "Scripts.h"



void TrapLevel::CreateTrapLevel()
{
	// Texture 로딩하기
	Ptr<CTexture> pNoiseTex = CAssetMgr::GetInst()->Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");
	Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(L"TileMapTex", L"Texture\\TILE.bmp");
	Ptr<CTexture> pmouse = CAssetMgr::GetInst()->Load<CTexture>(L"mouse", L"Texture\\mouse_2.png");
	Ptr<CTexture> pTurretTex = CAssetMgr::GetInst()->Load<CTexture>(L"TurretTex", L"Texture\\TrainingLevel_GrappleTurret_Hookable.png");
	Ptr<CTexture> FaceMark = CAssetMgr::GetInst()->Load<CTexture>(L"FaceMark", L"Texture\\PlayerFaceMark_1.png");

	CCollisionMgr::GetInst()->CollisionCheckClear();

	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(4, 5); // 플레이어 오브젝트랑 몬스터
	CCollisionMgr::GetInst()->CollisionCheck(3, 5); // 플레이어랑 몬스터
	CCollisionMgr::GetInst()->CollisionCheck(3, 7); // 플레이어와 충돌체
	CCollisionMgr::GetInst()->CollisionCheck(3, 6); // 플레이어와 몬스터 오브젝트
	CCollisionMgr::GetInst()->CollisionCheck(3, 8); // 플레이어와 서클 생성하는 발판
	CCollisionMgr::GetInst()->CollisionCheck(4, 6); // 플레이어 오브젝트랑 몬스터 오브젝트
	CCollisionMgr::GetInst()->CollisionCheck(9, 6); // 플레이어 오브젝트와 몬스터 오브젝트


	CLevel* m_CurLevel = new CLevel;
	m_CurLevel->SetName(L"Trap");

	ChangeLevel(m_CurLevel, LEVEL_STATE::STOP);

	m_CurLevel->GetLayer(0)->SetName(L"Background");
	m_CurLevel->GetLayer(1)->SetName(L"Tile");
	m_CurLevel->GetLayer(2)->SetName(L"Default");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"PlayerObject");
	m_CurLevel->GetLayer(5)->SetName(L"Monster");
	m_CurLevel->GetLayer(6)->SetName(L"MonsterObject");
	m_CurLevel->GetLayer(7)->SetName(L"Wall");
	m_CurLevel->GetLayer(10)->SetName(L"UI");

	CGameObject* pObject = nullptr;
	// ==========
	// SubCamera
	// ==========
	pObject = new CGameObject;
	pObject->SetName(L"SubCamera");
	pObject->AddComponent(new CCamera);
	// SubCamera 로 설정
	pObject->Camera()->SetPriority(0);
	// 모든 레이어를 찍도록 설정
	pObject->Camera()->LayerCheck(10);
	// 카메라 투영방식 설정
	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	m_CurLevel->AddObject(0, pObject, false);

	// ==========
	// MainCamera
	// ==========
	pObject = new CGameObject;
	pObject->SetName(L"MainCamera");
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCameraScript);

	// MainCamera 로 설정
	pObject->Camera()->SetPriority(1);

	// 모든 레이어를 찍도록 설정
	pObject->Camera()->SetLayerCheck(0x3FF);

	// 카메라 투영방식 설정
	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	m_CurLevel->AddObject(0, pObject, false);


	// =================
	// 테스트 추가
	// =================
	CGameObject* pTest = new CGameObject;
	pTest->SetName(L"UiBackground");
	pTest->AddComponent(new CMeshRender);
	pTest->Transform()->SetRelativePos(-444.f, 318.f, 9.f);
	pTest->Transform()->SetRelativeScale(315.f, 30.f, 1.f);
	pTest->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTest->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pTest->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\UiBackground.png"));
	CreateObject(pTest, 10, false);

	CGameObject* HpBar = new CGameObject;
	HpBar->SetName(L"HP");
	HpBar->AddComponent(new CMeshRender);
	HpBar->AddComponent(new CUI_Hpbar);

	HpBar->Transform()->SetRelativePos(-19.f, 0.f, 0.f);
	HpBar->Transform()->SetRelativeScale(267.f, 18.f, 1.f);
	HpBar->Transform()->SetIndependentScale(true);
	HpBar->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HpBar->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	HpBar->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\HP.png"));
	CreateObject(HpBar, 10, false);
	AddChild(pTest, HpBar);

	HpBar = new CGameObject;
	HpBar->SetName(L"Heal");
	HpBar->AddComponent(new CMeshRender);
	HpBar->Transform()->SetRelativePos(134.f, 0.f, 0.f);
	HpBar->Transform()->SetRelativeScale(50.f, 50.f, 1.f);
	HpBar->Transform()->SetIndependentScale(true);
	HpBar->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HpBar->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	HpBar->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\CureIcon.png"));
	CreateObject(HpBar, 10, false);
	AddChild(pTest, HpBar);

	// =================
	// 광원 오브젝트 추가
	// =================
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Light2D 1");
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(-100.f, 0.f, 10.f);
	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
	pLightObj->Light2D()->SetRadius(200.f);
	pLightObj->Light2D()->SetAngle(90.f);
	pLightObj->Light2D()->SetDir(Vec3(1.f, 0.f, 0.f));

	CreateObject(pLightObj, 0, false);

	// ======
	// Player
	// ======
	{
		CGameObject* pPlayer = new CGameObject;
		pPlayer->SetName(L"Player");
		pPlayer->AddComponent(new CMeshRender);
		pPlayer->AddComponent(new CPlayer_Ai);
		pPlayer->AddComponent(new CPlayerScript);
		pPlayer->AddComponent(new CPlayer_Dash);
		pPlayer->AddComponent(new CCollider2D);
		pPlayer->AddComponent(new CFlipbookPlayer);
		pPlayer->AddComponent(new CRigidBody);
		pPlayer->AddComponent(new CPlayer_Attack_Script1);
		pPlayer->AddComponent(new CPlayer_Attack_Script2);
		pPlayer->AddComponent(new CPlayer_Attack_Script3);
		pPlayer->AddComponent(new CPlayer_Arrow_Script);
		pPlayer->AddComponent(new CShield_Script);
		pPlayer->AddComponent(new CPlayer_Hook_Script);
		pPlayer->AddComponent(new CPlayer_Death);
		pPlayer->AddComponent(new CPlayer_BossRoom);
		pPlayer->AddComponent(new CPlayer_Main);

		pPlayer->Transform()->SetRelativePos(0.f, 0.f, 10.f);
		pPlayer->Transform()->SetRelativeScale(80.f, 80.f, 1.f);
		pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pPlayer->Collider2D()->SetScale(Vec2(0.3f, 0.6f));

		pPlayer->RigidBody()->SetFriction(10.f);
		pPlayer->RigidBody()->SetMass(1.f);

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Idle_Down, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Idle_Down"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Idle_Up, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Idle_Up"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Idle_Right, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Idle_Right"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Move_Down, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_move_down"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Move_Up, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_move_up"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Move_Right, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_move_right"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Down1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_down1"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Down2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_down2"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Down3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_down3"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Up1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_up1"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Up2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_up2"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Up3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_up3"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Right1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_Right1"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Right2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_Right2"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Atk_Right3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Atk_Right3"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Dash_Down, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Dash_Down"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Dash_Up, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Dash_Up"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Dash_Right, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Dash_Right"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Shield_Down, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Shield_Down"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Shield_Up, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Shield_Up"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Shield_Right, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Shield_Right"));

		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Death, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Death"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::appeared, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_appeared"));
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Warp, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_Warp"));


		pPlayer->FlipbookPlayer()->Play((UINT)Player_Ani_State::Idle_Down, 5.f, true);

		CreateObject(pPlayer, 3, false);

		// 자식으로 링 달아줌
		CGameObject* pPlayer_SircleRing = new CGameObject;
		pPlayer_SircleRing->SetName(L"Player_SircleRing");
		pPlayer_SircleRing->AddComponent(new CMeshRender);
		pPlayer_SircleRing->AddComponent(new CFaceMark_Script);

		pPlayer_SircleRing->Transform()->SetRelativePos(0.f, -0.35f, 1.f);
		pPlayer_SircleRing->Transform()->SetRelativeScale(0.4f, 0.4f, 1.f);
		pPlayer_SircleRing->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPlayer_SircleRing->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PlayerRingMtrl"));
		pPlayer_SircleRing->MeshRender()->GetDynamicMaterial();
		pPlayer_SircleRing->SetLayerIdx(3);

		CreateObject(pPlayer_SircleRing, 0, false);

		AddChild(pPlayer, pPlayer_SircleRing);

		// 자식으로 광원 달아줌
		pLightObj = new CGameObject;
		pLightObj->SetName(L"Boss Light2D 2");
		pLightObj->AddComponent(new CLight2D);

		pLightObj->Transform()->SetRelativePos(0.f, 0.f, 1.f);
		pLightObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
		pLightObj->Light2D()->SetLightColor(Vec3(0.2f, 0.2f, 0.2f));
		pLightObj->Light2D()->SetRadius(100.f);

		CreateObject(pLightObj, 0, false);
		AddChild(pPlayer, pLightObj);
	}

	Ptr<CTexture> GroundTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\Stage_Ground.png", L"Texture\\Map\\Stage_Ground.png");
	Ptr<CTexture> HoleTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\Stage_Cliff_0.png", L"Texture\\Map\\Stage_Cliff_0.png");
	Ptr<CTexture> DeepTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\Stage_Cliff_1.png", L"Texture\\Map\\Stage_Cliff_1.png");
	Ptr<CTexture> WallTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\Stage_WallGrid.png", L"Texture\\Map\\Stage_WallGrid.png");

	CGameObject* pWall = new CGameObject;
	pWall->SetName(L"Wall_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(-196.f, 420.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(1, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pWall);
	CAssetMgr::GetInst()->AddAsset(L"Wall", pPrefab);

	//pPrefab->CreatePrefab(L"Wall_2", Vec3(196.f, 420.f, 100.f), 7);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_2");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(196.f, 420.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(1, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_3");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(65.f, 1860.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_3_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(390.f, 2100.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_3_2");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(1560.f, 2100.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_4");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(975.f, 1860.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(7, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_5");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2210.f, 1860.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(8, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_6");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(1950.f, 540.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_7"); // 가운데 들어가는거
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2080.f, -95.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 415);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 0.86f));
	pWall->Collider2D()->SetOffset(Vec2(0.f, -0.05f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_8");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2600.f, 540.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_9");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2860.f, 660.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_10");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(3185.f, 540.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_12");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(7735.f, 60.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_13");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(11830.f, 0.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_14");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12285.f, 120.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);


	//=============================================================
	CGameObject* pGround = new CGameObject;
	pGround->SetName(L"Ground_1");
	pGround->AddComponent(new CRepeatSprite); 
	pGround->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_2");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(0.f, 900.f, 100.f));
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetRepeatCount(2, 12); 
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_2_1");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(390.f, 1740.f, 100.f));
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetRepeatCount(2, 2); 
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_2_2");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1560.f, 1740.f, 100.f));
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetRepeatCount(2, 2); 
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_3");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1430.f, 1500.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(20, 2); // 가로 5번, 세로 3번 반복
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_4");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(2340.f, 420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 16); // 가로 5번, 세로 3번 반복
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_5");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(2795.f, 180.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(17, 2); // 가로 5번, 세로 3번 반복
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_6");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1950.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 2); // 가로 5번, 세로 3번 반복
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_7");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1820.f, -120.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);



	pGround = new CGameObject;
	pGround->SetName(L"Ground_8"); // 중간에 삐져나온거
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(2860.f, 180.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 4);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_9"); // 이제 터렛 시작지점
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(3640.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);


	pWall = new CGameObject;
	pWall->SetName(L"Hole_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(3835.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(1, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);



	// 천장 2줄 붙는놈
	pWall = new CGameObject;
	pWall->SetName(L"Hole_2");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(4030.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);



	pGround = new CGameObject;
	pGround->SetName(L"Ground_10"); // 여기 터렛 있어야함 
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(4355.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);


	// 위에 그라운드 바닥에 붙는놈
	pWall = new CGameObject;
	pWall->SetName(L"Hole_3");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(4355.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);


	// 완료
	// 
	// 천장에 붙는 놈 3줄
	pWall = new CGameObject;
	pWall->SetName(L"Hole_4");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(4745.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_11"); // 여기 터렛 있어야함
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(5135.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 11 그라운드 바닥에 붙는놈
	pWall = new CGameObject;
	pWall->SetName(L"Hole_5");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(5135.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);


	// 11 그라운드 다음 천장에 붙는 놈 2줄
	pWall = new CGameObject;
	pWall->SetName(L"Hole_6");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(5460.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_12"); // 여기 터렛 있어야함
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(6045.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(7, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 그라운드 바닥에 붙는놈
	pWall = new CGameObject;
	pWall->SetName(L"Hole_7");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6050.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(7, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	// 12 그라운드 다음 천장에 붙는 놈 2줄
	pWall = new CGameObject;
	pWall->SetName(L"Hole_8");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6630.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_13");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(6890.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 13 그라운드 다음 바닥
	pWall = new CGameObject;
	pWall->SetName(L"Hole_9_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6890.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	///////////////////////////////////////////////////////////////////
	// 옆으로 엄청긴거
	pWall = new CGameObject;
	pWall->SetName(L"Wall_Long");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(10855.f, 660.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(115, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_Long");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12675.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(87, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Deep_Long");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(11114.f, -419.f, 102.f));
	pWall->RepeatSprite()->SetRepeatCount(111, 10);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pWall->RepeatSprite()->SetTexture(DeepTex);
	m_CurLevel->AddObject(0, pWall, false);
	///////////////////////////////////////////////////////////////////////

	pGround = new CGameObject;
	pGround->SetName(L"Ground_14"); //터렛
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(6890.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 14 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_10");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6890.f, -660.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_15"); // 터렛 14랑 간격 3칸차이
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(7540.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(6, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 15터렛 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_11");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(7540.f, -660.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(6, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	

	pGround = new CGameObject;
	pGround->SetName(L"Ground_16"); // 뒤에 쫒아 오는 거 생기는 광활한 곳
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(8515.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(9, 9);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);


	pGround = new CGameObject;
	pGround->SetName(L"Ground_17"); // 터렛으로 넘어가는 부분
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(9490.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 17 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_12");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(9490.f, -600.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_18"); // 터렛 17 이랑 3간격차이
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(10140.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 18 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_13");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(10140.f, -720.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_19"); // 터렛 18 이랑 2간격차이
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(10660.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 19 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_14");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(10660.f, -600.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_20"); // 터렛 19 이랑 2간격차이
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(11635.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(7, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);



	// 20 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_15");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(11635.f, -600.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(7, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_21"); // 20이랑 붙어있는 장판
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(12285.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 5);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 21 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_17");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12285.f, -840.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);


	pGround = new CGameObject;
	pGround->SetName(L"Ground_22"); // 21이랑 붙어있는 장판
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(12805.f, -480.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(5, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 22 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_18");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12805.f, -720.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(5, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_23"); // 22이랑 붙어있는 장판
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(13260.f, -240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(6, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 22 그라운드 아래
	pWall = new CGameObject;
	pWall->SetName(L"Hole_19");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(13390.f, -480.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_24"); // 22이랑 붙어있는 장판
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(13780.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 5);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_25");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(14170.f, -600.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_20");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(14170.f, -840.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(8, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_21");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(14820.f, -240.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_22");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(15340.f, -720.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(6, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_26");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(14560.f, -300.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 7);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_27");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(14950.f, 0.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_28");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(15080.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 4);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_29");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(15470.f, -480.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_30");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(15665.f, -180.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(1, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_31");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(15925.f, -60.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 1);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_32");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(16965.f, -60.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(13, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_33");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(18070.f, -60.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 5);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_23");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(15925.f, -240.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_24");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(16965.f, -360.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(13, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_25");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(18070.f, -480.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWallScript);
	pWall->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, pWall, false);

	// 스파이크 함정
	CGameObject* Spik = new CGameObject;
	Spik->SetName(L"Spik_1");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(-65.f, 600.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_2");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(65.f, 600.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_3");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(-65.f, 960.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_4");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(65.f, 960.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_5");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(-65.f, 1200.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_6");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(65.f, 1200.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_7");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(11635.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_8");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(11635.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_9");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(11895.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_10");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(11895.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_11");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(12545.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_12");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(12675.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_13");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(12805.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_14");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(12805.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_15");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13065.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_16");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13065.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_17");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13065.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_18");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13195.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_18");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13195.f, -180.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_19");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13455.f, -180.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_20");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13455.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_21");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13715.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_22");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13845.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_23");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13715.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_24");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(13975.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_25");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14105.f, -660.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_26");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14235.f, -660.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_27");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14625.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_28");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14495.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_29");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14495.f, -180.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_30");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14755.f, 60.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_31");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14885.f, 60.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_32");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(14885.f, -60.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_33");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(15015.f, -180.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_34");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(15015.f, -300.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_35");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(15145.f, -420.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_36");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(15145.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_37");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(15275.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);

	Spik = new CGameObject;
	Spik->SetName(L"Spik_38");
	Spik->AddComponent(new CSpikScript);
	Spik->AddComponent(new CMeshRender);
	Spik->AddComponent(new CCollider2D);
	Spik->Collider2D()->SetScale(Vec2(1.f, 1.f));
	Spik->Transform()->SetRelativePos(Vec3(15405.f, -540.f, 99.f));
	m_CurLevel->AddObject(5, Spik, false);



	// ============
	// TurretScript
	// ============

	CGameObject* pTurret = new CGameObject;
	pTurret->SetName(L"Turret1");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(4225.f, 260.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));
	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret2");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(5005.f, 260.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret3");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(5655.f, 260.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret4");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(6825.f, 260.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret5");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(6825.f, -325.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret6");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(7345.f, -325.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret7");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(9425.f, -325.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret8");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(10075.f, -380.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret9");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(10595.f, -325.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);

	pTurret = new CGameObject;
	pTurret->SetName(L"Turret10");
	pTurret->AddComponent(new CMeshRender);
	pTurret->AddComponent(new CTurretScript);
	pTurret->AddComponent(new CCollider2D);

	pTurret->Transform()->SetRelativePos(11245.f, -325.f, 10.f);
	pTurret->Transform()->SetRelativeScale(50.f, 100.f, 1.f);
	pTurret->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);
	//////////////////////////////////////////////////////////////////////////////////////////////////
	Ptr<CTexture> LockTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\LockDoor.png", L"Texture\\Map\\LockDoor.png");
	Ptr<CTexture> LockMarkTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\LockMark.png", L"Texture\\Map\\LockMark.png");
	Ptr<CTexture> UnLockMarkTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Map\\UnLockMark.png", L"Texture\\Map\\UnLockMark.png");


	// 충돌체 배치하기
	CGameObject* Lock = new CGameObject;
	Lock->SetName(L"Lock_1");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(0.f, -230.f, 10.f);
	Lock->Transform()->SetRelativeScale(600.f, 100.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_2");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(-310.f, 0.f, 10.f);
	Lock->Transform()->SetRelativeScale(100.f, 470.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_3");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(310.f, 0.f, 10.f);
	Lock->Transform()->SetRelativeScale(100.f, 470.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_4");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(-208.f, 960.f, 10.f);
	Lock->Transform()->SetRelativeScale(154.f, 1561.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_5");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(205.f, 780.f, 10.f);
	Lock->Transform()->SetRelativeScale(146.f, 1200.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_6");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(1238.f, 1312.f, 10.f);
	Lock->Transform()->SetRelativeScale(1930.f, 134.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_7");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(2600.f, 1080.f, 10.f);
	Lock->Transform()->SetRelativeScale(250.f, 590.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_8");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(2130.f, 1010.f, 10.f);
	Lock->Transform()->SetRelativeScale(150.f, 440.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_9");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(2814.f, 1493.f, 10.f);
	Lock->Transform()->SetRelativeScale(150.f, 264.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_10");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(1615.f, -125.f, 10.f);
	Lock->Transform()->SetRelativeScale(150.f, 845.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_11");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(1615.f, -125.f, 10.f);
	Lock->Transform()->SetRelativeScale(150.f, 845.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_12");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(2081.f, -610.f, 10.f);
	Lock->Transform()->SetRelativeScale(800.f, 150.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_13");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(2600.f, -240.f, 10.f);
	Lock->Transform()->SetRelativeScale(260.f, 600.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_14");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(2860.f, -125.f, 10.f);
	Lock->Transform()->SetRelativeScale(260.f, 133.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_15");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(2860.f, -125.f, 10.f);
	Lock->Transform()->SetRelativeScale(260.f, 133.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_16");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(3376.f, 1.f, 10.f);
	Lock->Transform()->SetRelativeScale(770.f, 112.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_17");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(4030.f, 121.f, 10.f);
	Lock->Transform()->SetRelativeScale(262.f, 113.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_18");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(4030.f, 121.f, 10.f);
	Lock->Transform()->SetRelativeScale(262.f, 113.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_19");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(4745.f, 121.f, 10.f);
	Lock->Transform()->SetRelativeScale(385.f, 113.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);


	Lock = new CGameObject;
	Lock->SetName(L"Lock_20");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(5461.f, 121.f, 10.f);
	Lock->Transform()->SetRelativeScale(260.f, 113.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_21");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(6630.f, 121.f, 10.f);
	Lock->Transform()->SetRelativeScale(260.f, 113.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);


	Lock = new CGameObject;
	Lock->SetName(L"Lock_22");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(7086.f, 240.f, 10.f);
	Lock->Transform()->SetRelativeScale(130.f, 354.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_23");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(7085.f, -357.f, 10.f);
	Lock->Transform()->SetRelativeScale(130.f, 354.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_24");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(6694.f, -357.f, 10.f);
	Lock->Transform()->SetRelativeScale(130.f, 354.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_25");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(9230.f, -357.f, 10.f);
	Lock->Transform()->SetRelativeScale(250.f, 1000.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_26");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(9815.f, -420.f, 10.f);
	Lock->Transform()->SetRelativeScale(389.f, 360.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_27");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(10401.f, -420.f, 10.f);
	Lock->Transform()->SetRelativeScale(260.f, 360.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_28");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(10986.f, -363.f, 10.f);
	Lock->Transform()->SetRelativeScale(390.f, 240.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_29");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(10460.f, -171.f, 10.f);
	Lock->Transform()->SetRelativeScale(2200.f, 140.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_30");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(12676.f, -238.f, 10.f);
	Lock->Transform()->SetRelativeScale(390.f, 240.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_31");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(13389.f, -53.f, 10.f);
	Lock->Transform()->SetRelativeScale(1040.f, 130.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_32");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(14170.f, -184.f, 10.f);
	Lock->Transform()->SetRelativeScale(519.f, 590.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_33");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(14820.f, -540.f, 10.f);
	Lock->Transform()->SetRelativeScale(255.f, 355.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_34");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(14820.f, -540.f, 10.f);
	Lock->Transform()->SetRelativeScale(255.f, 355.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_35");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(14820.f, 191.f, 10.f);
	Lock->Transform()->SetRelativeScale(777.f, 140.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_36");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(15405.f, -119.f, 10.f);
	Lock->Transform()->SetRelativeScale(390.f, 480.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_37");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(15925.f, -480.f, 10.f);
	Lock->Transform()->SetRelativeScale(390.f, 235.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_38");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(15859.f, 60.f, 10.f);
	Lock->Transform()->SetRelativeScale(520.f, 120.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_39");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(15859.f, 60.f, 10.f);
	Lock->Transform()->SetRelativeScale(520.f, 120.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_40");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(16959.f, 181.f, 10.f);
	Lock->Transform()->SetRelativeScale(1685.f, 120.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_41");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(18390.f, -60.f, 10.f);
	Lock->Transform()->SetRelativeScale(120.f, 610.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_42");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(18073.f, 280.f, 10.f);
	Lock->Transform()->SetRelativeScale(531.f, 90.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_43");
	Lock->AddComponent(new CRepeatSprite);
	Lock->Transform()->SetRelativePos(5200.f, 462.f, 10.f);
	Lock->Transform()->SetRelativeScale(3615.f, 80.f, 1.f);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	///////////////////////////////////////////////////////////////////////////
	CGameObject* LockDoor = new CGameObject;
	LockDoor->SetName(L"LockDoor_W1");
	LockDoor->AddComponent(new CRepeatSprite);
	LockDoor->AddComponent(new CWallScript);
	LockDoor->AddComponent(new CCollider2D);

	LockDoor->Transform()->SetRelativePos(2340.f, 330.f, 10.f);
	LockDoor->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, XM_PI / 2));
	LockDoor->Collider2D()->SetScale(Vec2(1.f, 1.f));
	LockDoor->RepeatSprite()->SetRepeatCount(1, 2);
	LockDoor->RepeatSprite()->SetSingleImageSize(50.f, 135.f);
	LockDoor->RepeatSprite()->SetTexture(LockTex);
	m_CurLevel->AddObject(7, LockDoor, false);

	Ptr<CTexture> Button = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Map\\Train_nButton.png");

	CGameObject* DoorOpen_Obj = new CGameObject;
	DoorOpen_Obj->SetName(L"LockDoor_W1_Open");
	DoorOpen_Obj->AddComponent(new CRepeatSprite);
	DoorOpen_Obj->AddComponent(new CCollider2D);
	DoorOpen_Obj->AddComponent(new CWallOpen);

	DoorOpen_Obj->Transform()->SetRelativePos(160.f, 130.f, 1.f);
	DoorOpen_Obj->Transform()->SetRelativeRotation(0.f, 0.f, XM_PI / 2);
	DoorOpen_Obj->Transform()->SetIndependentScale(true);
	DoorOpen_Obj->Collider2D()->SetScale(Vec2(0.4f, 1.f));
	DoorOpen_Obj->RepeatSprite()->SetRepeatCount(1, 1);
	DoorOpen_Obj->RepeatSprite()->SetSingleImageSize(150.f, 100.f);
	DoorOpen_Obj->RepeatSprite()->SetTexture(Button);
	m_CurLevel->AddObject(5, DoorOpen_Obj, false);

	AddChild(LockDoor, DoorOpen_Obj);


	LockDoor = new CGameObject;
	LockDoor->SetName(L"LockDoor_L1");
	LockDoor->AddComponent(new CRepeatSprite);
	LockDoor->AddComponent(new CCollider2D);
	LockDoor->AddComponent(new CWallScript);

	LockDoor->Transform()->SetRelativePos(2535.f, 180.f, 10.f);
	LockDoor->Collider2D()->SetScale(Vec2(1.f, 1.f));
	LockDoor->RepeatSprite()->SetRepeatCount(1, 2);
	LockDoor->RepeatSprite()->SetSingleImageSize(50.f, 125.f);
	LockDoor->RepeatSprite()->SetTexture(LockTex);
	m_CurLevel->AddObject(7, LockDoor, false);

	DoorOpen_Obj = new CGameObject;
	DoorOpen_Obj->SetName(L"LockDoor_L1_Open");
	DoorOpen_Obj->AddComponent(new CRepeatSprite);
	DoorOpen_Obj->AddComponent(new CCollider2D);
	DoorOpen_Obj->AddComponent(new CWallOpen);
	DoorOpen_Obj->Transform()->SetRelativePos(-807.f, -651.f, 1.f);
	DoorOpen_Obj->Transform()->SetIndependentScale(true);
	DoorOpen_Obj->Collider2D()->SetScale(Vec2(0.4f, 1.f));
	DoorOpen_Obj->RepeatSprite()->SetRepeatCount(1, 1);
	DoorOpen_Obj->RepeatSprite()->SetSingleImageSize(150.f, 100.f);
	DoorOpen_Obj->RepeatSprite()->SetTexture(Button);
	m_CurLevel->AddObject(5, DoorOpen_Obj, false);
	AddChild(LockDoor, DoorOpen_Obj);

	LockDoor = new CGameObject;
	LockDoor->SetName(L"LockDoor_L2");
	LockDoor->AddComponent(new CRepeatSprite);
	LockDoor->AddComponent(new CCollider2D);
	LockDoor->AddComponent(new CWallScript);
	LockDoor->Transform()->SetRelativePos(7900.f, -360.f, 10.f);
	LockDoor->Collider2D()->SetScale(Vec2(1.f, 1.f));
	LockDoor->RepeatSprite()->SetRepeatCount(1, 3);
	LockDoor->RepeatSprite()->SetSingleImageSize(50.f, 125.f);
	LockDoor->RepeatSprite()->SetTexture(LockTex);
	m_CurLevel->AddObject(7, LockDoor, false);

	DoorOpen_Obj = new CGameObject;
	DoorOpen_Obj->SetName(L"LockDoor_L2_Open");
	DoorOpen_Obj->AddComponent(new CRepeatSprite);
	DoorOpen_Obj->AddComponent(new CCollider2D);
	DoorOpen_Obj->AddComponent(new CWallOpen);
	DoorOpen_Obj->Transform()->SetRelativePos(-141.f, 130.f, 1.f);
	DoorOpen_Obj->Transform()->SetIndependentScale(true);
	DoorOpen_Obj->Collider2D()->SetScale(Vec2(0.4f, 1.f));
	DoorOpen_Obj->RepeatSprite()->SetRepeatCount(1, 1);
	DoorOpen_Obj->RepeatSprite()->SetSingleImageSize(150.f, 100.f);
	DoorOpen_Obj->RepeatSprite()->SetTexture(Button);
	m_CurLevel->AddObject(5, DoorOpen_Obj, false);
	AddChild(LockDoor, DoorOpen_Obj);

	LockDoor = new CGameObject;
	LockDoor->SetName(L"LockDoor_L3");
	LockDoor->AddComponent(new CRepeatSprite);
	LockDoor->AddComponent(new CCollider2D);
	LockDoor->AddComponent(new CWallScript);

	LockDoor->Transform()->SetRelativePos(17775.f, -50.f, 10.f);
	LockDoor->Collider2D()->SetScale(Vec2(1.f, 1.f));
	LockDoor->RepeatSprite()->SetRepeatCount(1, 3);
	LockDoor->RepeatSprite()->SetSingleImageSize(50.f, 125.f);
	LockDoor->RepeatSprite()->SetTexture(LockTex);
	m_CurLevel->AddObject(5, LockDoor, false);

	DoorOpen_Obj = new CGameObject;
	DoorOpen_Obj->SetName(L"LockDoor_L3_Open");
	DoorOpen_Obj->AddComponent(new CRepeatSprite);
	DoorOpen_Obj->AddComponent(new CCollider2D);
	DoorOpen_Obj->AddComponent(new CWallOpen);
	DoorOpen_Obj->Transform()->SetRelativePos(-95.f, 0.f, 1.f);
	DoorOpen_Obj->Transform()->SetIndependentScale(true);
	DoorOpen_Obj->Collider2D()->SetScale(Vec2(0.4f, 1.f));
	DoorOpen_Obj->RepeatSprite()->SetRepeatCount(1, 1);
	DoorOpen_Obj->RepeatSprite()->SetSingleImageSize(150.f, 100.f);
	DoorOpen_Obj->RepeatSprite()->SetTexture(Button);
	m_CurLevel->AddObject(5, DoorOpen_Obj, false);
	AddChild(LockDoor, DoorOpen_Obj);

	////////////////////////////////////////////////////////

	CGameObject* SircleTrap = new CGameObject;
	SircleTrap->SetName(L"SircleTrap_1");
	SircleTrap->AddComponent(new CSircleScript);
	SircleTrap->AddComponent(new CCollider2D);

	SircleTrap->Transform()->SetRelativePos(2337.f, 1000.f, 1.f); // 2338 -416
	SircleTrap->Transform()->SetRelativeScale(220.f, 100.f, 1.f);
	SircleTrap->Transform()->SetIndependentScale(true);
	SircleTrap->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(8, SircleTrap, false);

	SircleTrap = new CGameObject;
	SircleTrap->SetName(L"SircleTrap_2");
	SircleTrap->AddComponent(new CSircleScript);
	SircleTrap->AddComponent(new CCollider2D);
	SircleTrap->Transform()->SetRelativeScale(212.f, 237.f, 1.f);
	SircleTrap->Transform()->SetRelativePos(8778.f, -373.f, 1.f); // 8000 3개면 될듯
	SircleTrap->Transform()->SetIndependentScale(true);
	SircleTrap->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(8, SircleTrap, false);

	SircleTrap = new CGameObject;
	SircleTrap->SetName(L"SircleTrap_3");
	SircleTrap->AddComponent(new CSircleScript); //16000부터 시작
	SircleTrap->AddComponent(new CCollider2D);
	SircleTrap->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	SircleTrap->Transform()->SetRelativePos(15666.f, -63.f, 1.f);
	SircleTrap->Transform()->SetIndependentScale(true);
	SircleTrap->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(8, SircleTrap, false);

	SircleTrap = new CGameObject;
	SircleTrap->SetName(L"SircleTrap_4");
	SircleTrap->AddComponent(new CSircleScript); //16000부터 시작
	SircleTrap->AddComponent(new CCollider2D);
	SircleTrap->Transform()->SetRelativeScale(220.f, 50.f, 1.f);
	SircleTrap->Transform()->SetRelativePos(0.f, 300.f, 1.f);
	SircleTrap->Transform()->SetIndependentScale(true);
	SircleTrap->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(8, SircleTrap, false);

	CGameObject* Goal = new CGameObject;
	Goal->SetName(L"Goal");
	Goal->AddComponent(new CMeshRender); 
	Goal->AddComponent(new CCollider2D);
	Goal->AddComponent(new CTrap_Warp);
	Goal->Transform()->SetRelativeScale(65.f, 120.f, 1.f);
	Goal->Transform()->SetRelativePos(18071.f, 3.f, 11.f);
	Goal->Transform()->SetIndependentScale(true);
	Goal->MeshRender()->SetMesh(L"RectMesh");
	Goal->MeshRender()->SetMaterial(L"Std2DAlphaBlendMtrl");
	Goal->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Map\\TrainingLevel_FlagTower_Flag.png"));
	Goal->Collider2D()->SetScale(Vec2(1.f, 2.f));
	Goal->Collider2D()->SetOffset(Vec2(-3.f, -0.5f));
	m_CurLevel->AddObject(8, Goal, false);

	Goal = new CGameObject;
	Goal->SetName(L"Goal_Base");
	Goal->AddComponent(new CMeshRender);
	Goal->Transform()->SetRelativeScale(270.f, 270.f, 1.f);
	Goal->Transform()->SetRelativeRotation(XM_PI/3, 0.f, 0.f);
	Goal->Transform()->SetRelativePos(18071.f, -60.f, 11.f);
	Goal->Transform()->SetIndependentScale(true);
	Goal->MeshRender()->SetMesh(L"RectMesh");
	Goal->MeshRender()->SetMaterial(L"Std2DAlphaBlendMtrl");
	Goal->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Map\\TrainingLevel_FlagTower_Base.png"));
	m_CurLevel->AddObject(6, Goal, false);

	//TrainingLevel_FlagTower_Base

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 마우스 오브젝트 pmouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");
	pMouse->AddComponent(new CMouseScript);
	pMouse->AddComponent(new CMeshRender);
	pMouse->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"MouseMtrl"));
	pMouse->Transform()->SetRelativePos(g_MouseInfo.MousePos.x, g_MouseInfo.MousePos.y, 10.f);
	pMouse->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));

	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pmouse);

	m_CurLevel->AddObject(2, pMouse, false);

	// 레벨 상태 변경
	::ChangeLevelState(LEVEL_STATE::PLAY);
}


//{
//	// =======
//	// TileMap
//	// =======
//	CGameObject* pTileMap = new CGameObject;
//	pTileMap->SetName(L"TileMap");
//	pTileMap->AddComponent(new CTileMap);
//
//	pTileMap->Transform()->SetRelativePos(0.f, 0.f, 100.f);
//
//	pTileMap->TileMap()->SetAtlasTexture(pAtlasTex);
//	pTileMap->TileMap()->SetTileAtlasSize(64, 64);
//	pTileMap->TileMap()->SetTileSize(64.f, 64.f);
//	pTileMap->TileMap()->SetTileColRow(18, 18);
//
//	m_CurLevel->AddObject(1, pTileMap, false);
//
//
//	// 오른쪽 벽
//	CGameObject* pTileMap2 = new CGameObject;
//	pTileMap2->SetName(L"TileMap2");
//	pTileMap2->AddComponent(new CTileMap);
//	pTileMap2->AddComponent(new CCollider2D);
//	pTileMap2->AddComponent(new CWallScript);
//
//	pTileMap2->Transform()->SetRelativePos(600.f, 0.f, 100.f);
//	pTileMap2->Collider2D()->SetScale(Vec2(1.f, 1.f));
//	pTileMap2->TileMap()->SetAtlasTexture(pAtlasTex);
//	pTileMap2->TileMap()->SetTileAtlasSize(64, 64);
//	pTileMap2->TileMap()->SetTileSize(64.f, 64.f);
//	pTileMap2->TileMap()->SetTileColRow(1, 17);
//
//	m_CurLevel->AddObject(5, pTileMap2, false);
//
//	// 왼쪽 벽
//	pTileMap2 = new CGameObject;
//	pTileMap2->SetName(L"TileMap2");
//	pTileMap2->AddComponent(new CTileMap);
//	pTileMap2->AddComponent(new CCollider2D);
//	pTileMap2->AddComponent(new CWallScript);
//
//	pTileMap2->Transform()->SetRelativePos(-600.f, 0.f, 100.f);
//	pTileMap2->Collider2D()->SetScale(Vec2(1.f, 1.f));
//	pTileMap2->TileMap()->SetAtlasTexture(pAtlasTex);
//	pTileMap2->TileMap()->SetTileAtlasSize(64, 64);
//	pTileMap2->TileMap()->SetTileSize(64.f, 64.f);
//	pTileMap2->TileMap()->SetTileColRow(1, 17);
//
//	m_CurLevel->AddObject(5, pTileMap2, false);
//
//	// 위쪽 벽
//	pTileMap2 = new CGameObject;
//	pTileMap2->SetName(L"TileMap2");
//	pTileMap2->AddComponent(new CTileMap);
//	pTileMap2->AddComponent(new CCollider2D);
//	pTileMap2->AddComponent(new CWallScript);
//
//	pTileMap2->Transform()->SetRelativePos(0.f, 400.f, 100.f);
//	pTileMap2->Collider2D()->SetScale(Vec2(1.f, 1.f));
//	pTileMap2->TileMap()->SetAtlasTexture(pAtlasTex);
//	pTileMap2->TileMap()->SetTileAtlasSize(64, 64);
//	pTileMap2->TileMap()->SetTileSize(64.f, 64.f);
//	pTileMap2->TileMap()->SetTileColRow(17, 1);
//
//	m_CurLevel->AddObject(5, pTileMap2, false);
//
//	// 아래쪽 벽
//	pTileMap2 = new CGameObject;
//	pTileMap2->SetName(L"TileMap2");
//	pTileMap2->AddComponent(new CTileMap);
//	pTileMap2->AddComponent(new CCollider2D);
//	pTileMap2->AddComponent(new CWallScript);
//
//	pTileMap2->Transform()->SetRelativePos(0.f, -430.f, 100.f);
//	pTileMap2->Collider2D()->SetScale(Vec2(1.f, 1.f));
//	pTileMap2->TileMap()->SetAtlasTexture(pAtlasTex);
//	pTileMap2->TileMap()->SetTileAtlasSize(64, 64);
//	pTileMap2->TileMap()->SetTileSize(64.f, 64.f);
//	pTileMap2->TileMap()->SetTileColRow(17, 1);
//
//	m_CurLevel->AddObject(5, pTileMap2, false);
//	}