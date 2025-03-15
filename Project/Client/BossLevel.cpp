#include "pch.h"
#include "BossLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>
#include "Scripts.h"

void BossLevel::CreateBossLevel()
{
	CLevel* m_CurLevel = new CLevel;

	// 테스트 레벨을 현재 레벨로 지정
	ChangeLevel(m_CurLevel, LEVEL_STATE::STOP);
	m_CurLevel->SetName(L"Boss");

	// Sound 로딩하기
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\ilodolly.wav", L"Sound\\ilodolly.wav");
	pSound->Play(0, 0.5f, true);

	CCollisionMgr::GetInst()->CollisionCheckClear();
	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(3, 6); // 플레이어와 몬스터 오브젝트
	CCollisionMgr::GetInst()->CollisionCheck(4, 5); // 몬스터와 플레이어 오브젝트
	CCollisionMgr::GetInst()->CollisionCheck(4, 6); // 플레이어 오브젝트와 몬스터 오브젝트
	CCollisionMgr::GetInst()->CollisionCheck(9, 6); // 플레이어 오브젝트와 몬스터 오브젝트

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
	// 플레이어 체력
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
	// 보스 체력
	// =================
	pTest = new CGameObject;
	pTest->SetName(L"BossHPBackground");
	pTest->AddComponent(new CMeshRender);
	pTest->Transform()->SetRelativePos(0.f, -318.f, 9.f);
	pTest->Transform()->SetRelativeScale(600.f, 30.f, 1.f);
	pTest->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pTest->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	pTest->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\UiBackground.png"));
	CreateObject(pTest, 10, false);

	HpBar = new CGameObject;
	HpBar->SetName(L"BossHP");
	HpBar->AddComponent(new CMeshRender);
	HpBar->AddComponent(new CUI_BossHP);

	HpBar->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	HpBar->Transform()->SetRelativeScale(600.f, 18.f, 1.f);
	HpBar->Transform()->SetIndependentScale(true);
	HpBar->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HpBar->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	HpBar->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\HP.png"));
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
		pPlayer->AddComponent(new CPlayer_End);

		
		pPlayer->Transform()->SetRelativePos(0.f, 0.f, 10.f);
		pPlayer->Transform()->SetRelativeScale(80.f, 80.f, 1.f);
		pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pPlayer->Collider2D()->SetScale(Vec2(0.3f, 0.8f));

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
		pPlayer->FlipbookPlayer()->AddFlipbook((UINT)Player_Ani_State::EndBoss, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Player_End"));

		
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
		pLightObj->Light2D()->SetRadius(200.f);

		CreateObject(pLightObj, 0, false);
		AddChild(pPlayer, pLightObj);
	}

	CGameObject* pBoss = new CGameObject;
	pBoss->SetName(L"Boss");
	pBoss->AddComponent(new CMeshRender);
	pBoss->AddComponent(new CBoss_Ai);
	pBoss->AddComponent(new CBoss_Idle);
	pBoss->AddComponent(new CBoss_Nor_Atk1);
	pBoss->AddComponent(new CBoss_Nor_Atk2);
	pBoss->AddComponent(new CBoss_Nor_Atk3);
	pBoss->AddComponent(new CBoss_Bottom_Atk);
	pBoss->AddComponent(new CBoss_Charge_Atk);
	pBoss->AddComponent(new CBoss_Jump_Atk);
	pBoss->AddComponent(new CBoss_Jump_landing);
	pBoss->AddComponent(new CBoss_Dash);
	pBoss->AddComponent(new CBoss_Groggy);
	pBoss->AddComponent(new CBoss_Dead);
	pBoss->AddComponent(new CBoss_Roar);

	
	pBoss->AddComponent(new CCollider2D);
	pBoss->AddComponent(new CFlipbookPlayer);
	pBoss->AddComponent(new CRigidBody);

	pBoss->Transform()->SetRelativePos(0.f, 0.f, 11.f);
	pBoss->Transform()->SetRelativeScale(550.f, 550.f, 1.f);
	pBoss->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBoss->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossMtrl"));
	pBoss->Collider2D()->SetScale(Vec2(0.5f, 0.5f));

	pBoss->RigidBody()->SetFriction(1.f);
	pBoss->RigidBody()->SetMass(5.f);
	pBoss->RigidBody()->SetMaxSpeed(10000.f);

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
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Dash, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Dash"));
	pBoss->FlipbookPlayer()->AddFlipbook((UINT)Boss_Ani_State::Boss_Execute, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Execute"));

	pBoss->FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Idle, 10.f, true);
	CreateObject(pBoss, 5, false);

	// 마우스 오브젝트 pmouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");
	pMouse->AddComponent(new CMouseScript);
	pMouse->AddComponent(new CMeshRender);
	pMouse->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"MouseMtrl"));
	pMouse->Transform()->SetRelativePos(g_MouseInfo.MousePos.x, g_MouseInfo.MousePos.y, 10.f);
	pMouse->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));
	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\mouse_2.png"));
	m_CurLevel->AddObject(2, pMouse, false);

	CGameObject* pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Main");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Main.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Wall");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 380.f, 110.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 1820.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Wall.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Elervator");
	pMap->AddComponent(new CEB);
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, -371.f, 101.f)); //-371~ -243
	pMap->Transform()->SetRelativeScale(Vec3(124.f, 126.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Elervator.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_Down");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_0.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_1");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_1.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_2");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_2.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_3");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_3.png"));
	m_CurLevel->AddObject(0, pMap, false);


	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_4");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_4.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_R1");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(-1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_1.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_R2");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(-1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_2.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_R3");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(-1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_3.png"));
	m_CurLevel->AddObject(0, pMap, false);


	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_R4");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(-1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_4.png"));
	m_CurLevel->AddObject(0, pMap, false);

	pMap = new CGameObject;
	pMap->SetName(L"Stage_Berserker_Floor_Edge_Up");
	pMap->AddComponent(new CMeshRender);
	pMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pMap->Transform()->SetRelativeScale(Vec3(1100.f, 900.f, 1.f));
	pMap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	pMap->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BossMap\\Stage_Berserker_Floor_Edge_5.png"));
	m_CurLevel->AddObject(0, pMap, false);



	// 레벨 상태 변경
	::ChangeLevelState(LEVEL_STATE::PLAY);
}
