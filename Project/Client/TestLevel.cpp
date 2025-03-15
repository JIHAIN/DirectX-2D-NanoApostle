#include "pch.h"
#include "TestLevel.h"

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



void TestLevel::CreateTestLevel()
{
	// Texture �ε��ϱ�
	Ptr<CTexture> pNoiseTex = CAssetMgr::GetInst()->Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");
	Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(L"TileMapTex", L"Texture\\TILE.bmp");
	Ptr<CTexture> pmouse = CAssetMgr::GetInst()->Load<CTexture>(L"mouse", L"Texture\\mouse_2.png");
	Ptr<CTexture> pTurretTex = CAssetMgr::GetInst()->Load<CTexture>(L"TurretTex", L"Texture\\TrainingLevel_GrappleTurret_Hookable.png");
	Ptr<CTexture> FaceMark = CAssetMgr::GetInst()->Load<CTexture>(L"FaceMark", L"Texture\\PlayerFaceMark_1.png");

	CLevel* m_CurLevel = new CLevel;

	// �׽�Ʈ ������ ���� ������ ����
	ChangeLevel(m_CurLevel, LEVEL_STATE::STOP);

	m_CurLevel->GetLayer(0)->SetName(L"Background");
	m_CurLevel->GetLayer(1)->SetName(L"Tile");
	m_CurLevel->GetLayer(2)->SetName(L"Default");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"PlayerObject");
	m_CurLevel->GetLayer(5)->SetName(L"Monster");
	m_CurLevel->GetLayer(6)->SetName(L"MonsterObject");

	CGameObject* pObject = nullptr;

	// ==========
	// MainCamera
	// ==========
	pObject = new CGameObject;
	pObject->SetName(L"MainCamera");
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCameraScript);

	// MainCamera �� ����
	pObject->Camera()->SetPriority(0);

	// ��� ���̾ �ﵵ�� ����
	pObject->Camera()->LayerCheckAll();

	// ī�޶� ������� ����
	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	m_CurLevel->AddObject(0, pObject, false);

	// =================
	// ���� ������Ʈ �߰�
	// =================
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Light2D 1");
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(-100.f, 0.f, 10.f);
	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(200.f);
	pLightObj->Light2D()->SetAngle(90.f);
	pLightObj->Light2D()->SetDir(Vec3(1.f, 0.f, 0.f));

	CreateObject(pLightObj, 0, false);

	// CG �̺�Ʈ
	//CGameObject* pCG = new CGameObject;
	//pCG->SetName(L"CG 1");
	//pCG->AddComponent(new CFlipbookPlayer);
	//pCG->AddComponent(new CMeshRender);
	//pCG->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pCG->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	//pCG->Transform()->SetRelativePos(0.f, 0.f, 1.f);
	//pCG->Transform()->SetRelativeScale(1280.f, 780.f, 1.f);
	//pCG->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"CG"));
	//pCG->FlipbookPlayer()->Play(0, 1.f, true);
	//CreateObject(pCG, 0, false);

	// ======
	// Boss
	// ======
	// ���� �������� 400 , 400�� �ݶ��̴� 0.5 0.5
	CGameObject* pBoss = new CGameObject;
	pBoss->SetName(L"Player");
	pBoss->AddComponent(new CMeshRender);
	pBoss->AddComponent(new CBoss_Ai);
	pBoss->AddComponent(new CBoss_Idle);
	pBoss->AddComponent(new CBoss_Nor_Atk1);
	pBoss->AddComponent(new CBoss_Nor_Atk2);
	pBoss->AddComponent(new CBoss_Nor_Atk3);
	pBoss->AddComponent(new CBoss_Bottom_Atk);
	pBoss->AddComponent(new CBoss_Charge_Atk);
	pBoss->AddComponent(new CBoss_Jump_Atk);

	pBoss->AddComponent(new CCollider2D);
	pBoss->AddComponent(new CFlipbookPlayer);
	pBoss->AddComponent(new CRigidBody);

	pBoss->Transform()->SetRelativePos(0.f, 0.f, 10.f);
	pBoss->Transform()->SetRelativeScale(400.f, 400.f, 1.f);
	pBoss->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBoss->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pBoss->Collider2D()->SetScale(Vec2(0.5f, 0.5f));

	pBoss->RigidBody()->SetFriction(1.f);
	pBoss->RigidBody()->SetMass(1.f);
	pBoss->RigidBody()->SetMaxSpeed(5000.f);

	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Bottom_Atk, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Bottom_Atk"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Charge_Atk, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Charge_Atk"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Groggy, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Groggy"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Groggying, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Groggying"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Jump_Atk, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Jump_Atk"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Jump_Atk_landing, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Jump_Atk_landing"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Nor_Atk1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Nor_Atk1"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Nor_Atk2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Nor_Atk2"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Nor_Atk3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Nor_Atk3"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Idle, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Idle"));

	pBoss->FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Idle, 10.f, true);
	CreateObject(pBoss, 6, false);

	///////////// ������ ��� �����ڵ�//////////////////
	// ������ ���
	//Ptr<CPrefab> pPrefab = new CPrefab;

	//pPrefab->SetProtoObject(pBoss);
	////pPrefab->CreatePrefab();
	//CAssetMgr::GetInst()->AddAsset(L"Boss", pPrefab);

	//Ptr<CPrefab> m_Prefab = new CPrefab;
	//m_Prefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"Boss");
	//m_Prefab->CreatePrefab(L"Boss", Vec3(0.f,0.f,1.f), 6);
	///////////////////////////////////////////////////////////
	
	//// �ڽ����� ���� �޾���
	//pLightObj = new CGameObject;
	//pLightObj->SetName(L"Boss Light2D 2");
	//pLightObj->AddComponent(new CLight2D);

	//pLightObj->Transform()->SetRelativePos(0.f, 0.f, 1.f);
	//pLightObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pLightObj->Light2D()->SetLightColor(Vec3(0.6f, 0.6f, 1.f));
	//pLightObj->Light2D()->SetRadius(200.f);

	//CreateObject(pLightObj, 0, false);
	//AddChild(pBoss, pLightObj);

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

		pPlayer->Transform()->SetRelativePos(0.f, 0.f, 10.f);
		pPlayer->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
		pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pPlayer->Collider2D()->SetScale(Vec2(0.3f, 0.8f));

		pPlayer->RigidBody()->SetFriction(10.f);
		pPlayer->RigidBody()->SetMass(1.f);

		//pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Idle_Down, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerIdle_Down"));
		//pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Idle_Up, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerIdle_Up"));
		//pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::Idle_Right, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerIdle_Right"));

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


		pPlayer->FlipbookPlayer()->Play((UINT)Player_Ani_State::Idle_Down, 5.f, true);

		CreateObject(pPlayer, 3, false);

		// �ڽ����� �� �޾���
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
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_3_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(390.f, 2100.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_3_2");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(1560.f, 2100.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_4");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(975.f, 1860.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(7, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_5");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2210.f, 1860.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(8, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);



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
	pGround->Transform()->SetRelativePos(Vec3(0.f , 900.f, 100.f));
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetRepeatCount(2, 12); \
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_2_1");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(390.f, 1740.f, 100.f));
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetRepeatCount(2, 2);\
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_2_2");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1560.f, 1740.f, 100.f));
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetRepeatCount(2, 2); \
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_3");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1430.f, 1500.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(20, 2); // ���� 5��, ���� 3�� �ݺ�
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_4");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(2340.f, 420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 16); // ���� 5��, ���� 3�� �ݺ�
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_5");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(2795.f, 180.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(17, 2); // ���� 5��, ���� 3�� �ݺ�
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_6");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(1950.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 2); // ���� 5��, ���� 3�� �ݺ�
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

	pWall = new CGameObject;
	pWall->SetName(L"Wall_6");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(1950.f, 540.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_7"); // ��� ���°�
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2080.f, -120.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 3);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_8");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2600.f, 540.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_8"); // �߰��� �������°�
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(2860.f, 180.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 4);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_9"); // ���� �ͷ� ��������
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(3640.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(4, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_9");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(2860.f, 660.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_10");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(3185.f, 540.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(3835.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(1, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	// õ�� 2�� �ٴ³�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_2");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(4030.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);


	pGround = new CGameObject;
	pGround->SetName(L"Ground_10"); // ���� �ͷ� �־���� 
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(4355.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);
	
	
	// ���� �׶��� �ٴڿ� �ٴ³�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_3");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(4355.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	// �Ϸ�
	// 
	// õ�忡 �ٴ� �� 3��
	pWall = new CGameObject;
	pWall->SetName(L"Hole_4");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(4745.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_11"); // ���� �ͷ� �־����
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(5135.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 11 �׶��� �ٴڿ� �ٴ³�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_5");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(5135.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	// 11 �׶��� ���� õ�忡 �ٴ� �� 2��
	pWall = new CGameObject;
	pWall->SetName(L"Hole_6");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(5460.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);


	pGround = new CGameObject;
	pGround->SetName(L"Ground_12"); // ���� �ͷ� �־����
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(6045.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(7, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// �׶��� �ٴڿ� �ٴ³�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_7");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6050.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(7, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	// 12 �׶��� ���� õ�忡 �ٴ� �� 2��
	pWall = new CGameObject;
	pWall->SetName(L"Hole_8");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6630.f, 300.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_13");
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(6890.f, 240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 13 �׶��� ���� �ٴ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_9_1");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6890.f, -60.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	///////////////////////////////////////////////////////////////////
	// ������ ��û���
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
	pWall->Transform()->SetRelativePos(Vec3(12675.f, -419.f, 102.f));
	pWall->RepeatSprite()->SetRepeatCount(87, 10);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pWall->RepeatSprite()->SetTexture(DeepTex);
	m_CurLevel->AddObject(0, pWall, false);
	///////////////////////////////////////////////////////////////////////

	pGround = new CGameObject;
	pGround->SetName(L"Ground_14"); //�ͷ�
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(6890.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 14 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_10");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(6890.f, -660.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_15"); // �ͷ� 14�� ���� 3ĭ����
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(7540.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(6, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 15�ͷ� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_11");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(7540.f, -660.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(6, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_12");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(7735.f, 60.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_16"); // �ڿ� �i�� ���� �� ����� ��Ȱ�� ��
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(8515.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(9, 9);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);


	pGround = new CGameObject;
	pGround->SetName(L"Ground_17"); // �ͷ����� �Ѿ�� �κ�
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(9490.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 17 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_12");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(9490.f, -600.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_18"); // �ͷ� 17 �̶� 3��������
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(10140.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 3);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 18 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_13");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(10140.f, -720.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_19"); // �ͷ� 18 �̶� 2��������
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(10660.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(2, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 19 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_14");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(10660.f, -600.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_20"); // �ͷ� 19 �̶� 2��������
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(11635.f, -360.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(7, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_13");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(11830.f, 0.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	// 20 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_15");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(11635.f, -600.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(7, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_21"); // 20�̶� �پ��ִ� ����
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(12285.f, -420.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(3, 5);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 21 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_17");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12285.f, -840.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Wall_14");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12285.f, 120.f, 100.f));
	pWall->RepeatSprite()->SetRepeatCount(3, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 4);
	pWall->RepeatSprite()->SetTexture(WallTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_22"); // 21�̶� �پ��ִ� ����
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(12805.f, -480.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(5, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 22 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_18");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(12805.f, -720.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(5, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_23"); // 22�̶� �پ��ִ� ����
	pGround->AddComponent(new CRepeatSprite);
	pGround->Transform()->SetRelativePos(Vec3(13260.f, -240.f, 100.f));
	pGround->RepeatSprite()->SetRepeatCount(6, 2);
	pGround->RepeatSprite()->SetSingleImageSize(130.f, 120.f);
	pGround->RepeatSprite()->SetTexture(GroundTex);
	m_CurLevel->AddObject(0, pGround, false);

	// 22 �׶��� �Ʒ�
	pWall = new CGameObject;
	pWall->SetName(L"Hole_19");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(13390.f, -480.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pGround = new CGameObject;
	pGround->SetName(L"Ground_24"); // 22�̶� �پ��ִ� ����
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
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_21");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(14820.f, -240.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(2, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_22");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(15340.f, -720.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(6, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

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
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_24");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(16965.f, -360.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(13, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	pWall = new CGameObject;
	pWall->SetName(L"Hole_25");
	pWall->AddComponent(new CRepeatSprite);
	pWall->Transform()->SetRelativePos(Vec3(18070.f, -480.f, 101.f));
	pWall->RepeatSprite()->SetRepeatCount(4, 1);
	pWall->RepeatSprite()->SetSingleImageSize(130.f, 120.f * 2);
	pWall->RepeatSprite()->SetTexture(HoleTex);
	m_CurLevel->AddObject(0, pWall, false);

	// ������ũ ����
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

	pTurret->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTurret->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pTurret->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTurretTex);

	m_CurLevel->AddObject(5, pTurret, false);



	// ���콺 ������Ʈ pmouse
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

	//// Particle Object
	//CGameObject* pParticle = new CGameObject;
	//pParticle->SetName(L"Particle");
	//pParticle->AddComponent(new CParticleSystem);
	//pParticle->ParticleSystem()->SetParticleTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"PTex"));
	//CreateObject(pParticle, 2, false);

	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(3, 5); // �÷��̾�� ����
	CCollisionMgr::GetInst()->CollisionCheck(3, 6); // �÷��̾�� ���� ������Ʈ
	CCollisionMgr::GetInst()->CollisionCheck(4, 6); // �÷��̾� ������Ʈ�� ���� ������Ʈ

	// ���� ���� ����
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
//	// ������ ��
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
//	// ���� ��
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
//	// ���� ��
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
//	// �Ʒ��� ��
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