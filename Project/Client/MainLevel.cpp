#include "pch.h"
#include "MainLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>
#include "Scripts.h"


void MainLevel::CreateMainLevel()
{
	CLevel* m_CurLevel = new CLevel;
	m_CurLevel->SetName(L"Main");

	// 테스트 레벨을 현재 레벨로 지정
	ChangeLevel(m_CurLevel, LEVEL_STATE::STOP);
	//CLevelMgr::GetInst()->AddLevel(LEVEL_TYPE::MAIN, m_CurLevel);
	m_CurLevel->GetLayer(0)->SetName(L"Background");
	m_CurLevel->GetLayer(1)->SetName(L"Tile");
	m_CurLevel->GetLayer(2)->SetName(L"Default");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"PlayerObject");
	m_CurLevel->GetLayer(5)->SetName(L"Monster");
	m_CurLevel->GetLayer(6)->SetName(L"MonsterObject");
	m_CurLevel->GetLayer(7)->SetName(L"Wall");
	m_CurLevel->GetLayer(10)->SetName(L"UI");

	CCollisionMgr::GetInst()->CollisionCheckClear();

	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(3, 7); // 플레이어와 충돌체
	CCollisionMgr::GetInst()->CollisionCheck(3, 8); // 플레이어와 충돌체
	CCollisionMgr::GetInst()->CollisionCheck(9, 6); // 플레이어 오브젝트와 몬스터 오브젝트


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
	pLightObj->Light2D()->SetLightColor(Vec3(0.55f, 0.55f, 0.55f));
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
		pPlayer->Collider2D()->SetScale(Vec2(0.3f, 0.4f));

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
		pLightObj->Light2D()->SetLightColor(Vec3(0.3f, 0.3f, 0.3f));
		pLightObj->Light2D()->SetRadius(400.f);

		CreateObject(pLightObj, 0, false);
		AddChild(pPlayer, pLightObj);
	}

	CGameObject* pMap = new CGameObject;
	pMap->SetName(L"Platform_Front");
	pMap->AddComponent(new CMeshRender);
	pMap->AddComponent(new CMap_BossMain);
	pMap->AddComponent(new CCollider2D);

	pMap->Transform()->SetRelativePos(Vec3(0.f, -290.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(800.f, 900.f, 1.f));
	pMap->Collider2D()->IsIndependentScale();
	pMap->Collider2D()->SetScale(Vec2(0.1f, 0.1f));
	pMap->Collider2D()->SetOffset(Vec2(0.f, 0.3f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Platform_Front.png"));
	m_CurLevel->AddObject(7, pMap, false);
	

	pMap = new CGameObject;
	pMap->SetName(L"Platform_Training");
	pMap->AddComponent(new CMeshRender);
	pMap->AddComponent(new CMap_Main);
	pMap->AddComponent(new CCollider2D);

	pMap->Transform()->SetRelativePos(Vec3(343.f, 340.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(250.f, 300.f, 1.f));
	pMap->Collider2D()->IsIndependentScale();
	pMap->Collider2D()->SetScale(Vec2(0.3f, 0.3f));
	pMap->Collider2D()->SetOffset(Vec2(0.05f, 0.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Platform_Training.png"));
	m_CurLevel->AddObject(7, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Platform_Back");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 246.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(400.f, 500.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Platform_Back.png"));
	m_CurLevel->AddObject(0, pMap, false);



	pMap = new CGameObject;
	pMap->SetName(L"PlatformConnect");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(197.f, 339.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(70.f, 140.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\PlatformConnect.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Platform_Bottom");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(117.f, 132.f, 101.f));
	pMap->Transform()->SetRelativeScale(Vec3(1860.f, 1611.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Platform_Bottom.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Tower_0");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-580.f, -395.f, 101.f));
	pMap->Transform()->SetRelativeScale(Vec3(360.f, 840.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TowerMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Tower_0.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Tower_1");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-720.f, 246.f, 101.f));
	pMap->Transform()->SetRelativeScale(Vec3(700.f, 1450.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TowerMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Tower_1.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Tower_2");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(672.f, 288.f, 101.f));
	pMap->Transform()->SetRelativeScale(Vec3(546.f, 1400.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TowerMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Tower_2.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Tower_3");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(780.f, -30.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(250.f, 1300.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TowerMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Tower_3.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Tower_4");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(170.f, 540.f, 101.f));
	pMap->Transform()->SetRelativeScale(Vec3(615.f, 1300.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TowerMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Tower_4.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Tower_5");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 830.f, 102.f));
	pMap->Transform()->SetRelativeScale(Vec3(1280.f, 544.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TowerMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Tower_5.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"MainScreens_White_More");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-4.f, 567.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(600.f, 400.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\MainScreens_White_More.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"MainKeyboard");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 471.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(250.f, 75.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\MainKeyboard.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"MainKeyboard_Shadow");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 432.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(250.f, 75.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\MainKeyboard_Shadow.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"leftKeyboard");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-182.f, 346.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(40.f, 100.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\leftKeyboard.png"));
	m_CurLevel->AddObject(0, pMap, false);
	

	pMap = new CGameObject;
	pMap->SetName(L"leftScreen");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-230.f, 374.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(50.f, 100.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\leftScreen.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"TrainingScreen");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(364.f, 519.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(150.f, 100.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\TrainingScreen.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"leftKeyboard_Shine_0");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-157.f, 333.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(73.f, 114.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\leftKeyboard_Shine_0.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"leftKeyboard_Shine_1");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-170.f, 333.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(85.f, 114.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\leftKeyboard_Shine_1.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Blocks_Front");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(-45.f, 196.f, 99.f));
	pMap->Transform()->SetRelativeScale(Vec3(1543.f, 421.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Blocks_Front.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Blocks_Back");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(80.f, 430.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1000.f, 430.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\MainMap\\Blocks_Back.png"));
	m_CurLevel->AddObject(0, pMap, false);

	// 충돌체 배치하기
	CGameObject* Lock = new CGameObject;
	Lock->SetName(L"Lock_1");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(0.f, -200.f, 10.f);
	Lock->Transform()->SetRelativeScale(550.f, 70.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_2");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(-290.f, -20.f, 10.f);
	Lock->Transform()->SetRelativeScale(70.f, 350.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_3");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(290.f, -20.f, 10.f);
	Lock->Transform()->SetRelativeScale(70.f, 350.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_4");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(-177.f, 180.f, 10.f);
	Lock->Transform()->SetRelativeScale(130.f, 144.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_5");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(-200.f, 380.f, 10.f);
	Lock->Transform()->SetRelativeScale(70.f, 250.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);


	Lock = new CGameObject;
	Lock->SetName(L"Lock_6");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(10.f, 470.f, 10.f);
	Lock->Transform()->SetRelativeScale(370.f, 70.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_7");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(332.f, 449.f, 10.f);
	Lock->Transform()->SetRelativeScale(285.f, 70.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_8");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(504.f, 346.f, 10.f);
	Lock->Transform()->SetRelativeScale(70.f, 200.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);

	Lock = new CGameObject;
	Lock->SetName(L"Lock_9");
	Lock->AddComponent(new CRepeatSprite);
	Lock->AddComponent(new CCollider2D);
	Lock->AddComponent(new CWallScript);
	Lock->Transform()->SetRelativePos(297.f, 192.f, 10.f);
	Lock->Transform()->SetRelativeScale(370.f, 150.f, 1.f);
	Lock->Collider2D()->SetScale(Vec2(1.f, 1.f));
	m_CurLevel->AddObject(7, Lock, false);


	Ptr<CTexture> pmouse = CAssetMgr::GetInst()->Load<CTexture>(L"mouse", L"Texture\\mouse_2.png");

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
