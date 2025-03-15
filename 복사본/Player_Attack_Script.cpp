#include "pch.h"
#include "Player_Attack_Script.h"

#include "Player_Atk_Obj.h"

Player_Attack_Script::Player_Attack_Script()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT)
	, m_AtkDmg(0)
	, m_AtkRange(20)
	, m_AtkTime(0.f)
	, m_AtkCool(true)
	, m_Atk2Time(0.7f)
	, MousDir(0.f,0.f,0.f)
	, m_AtkCool_2(false)
{
}

Player_Attack_Script::~Player_Attack_Script()
{
}

void Player_Attack_Script::Begin()
{

}

void Player_Attack_Script::Tick()
{
	//GetOwner()->FlipbookPlayer()->Play((int)Player_Ani_State::Atk_Down, 10.f, false);
	//GetOwner()->FlipbookPlayer()->Play((int)Player_Ani_State::Atk_Up, 10.f, false);
	//GetOwner()->FlipbookPlayer()->Play((int)Player_Ani_State::Atk_Right, 10.f, false);

	Vec3 MousePos = g_MouseInfo.MouseWorldPos;

	Vec3 pWPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 PlayerMouseDir = MousePos - pWPos;
	MousDir = PlayerMouseDir.Normalize();

	if (!m_AtkCool) // 어택 을 했을때 다음 공격을 위해서 쿨타임 돌림
	{
		m_AtkTime += DT; // 어택 불가때 쿨타임 계산
		m_Atk2Time += DT; // 어택 불가는 어택을 하는 중이라 판단하고 0.7초 안에 하라고 DT 깜
	}

	if (m_AtkTime >= 1.f && !m_AtkCool_2) // 1초 마다 어택 쿨 돌기. 3타를 할 수 있을때는 돌지않음 
	{
		m_AtkCool = true;
	}

	if (KEY_TAP(KEY::LBTN) && m_AtkCool) // 이게 첫 공격
	{
		CreateAtk1();
		m_AtkCool = false;
		m_AtkTime = 0.f;
		m_Atk2Time = 0.f;

		GetOwner()->FlipbookPlayer()->Play((int)Player_Ani_State::Atk_Right, 10.f, false);
	}

	if (KEY_TAP(KEY::LBTN) && !m_AtkCool && m_Atk2Time >= 0.2f && m_Atk2Time <= 1.3f) // 2번째 공격 
	{
		CreateAtk2();
		m_Atk2Time = 1.5f; // 공격을 하고 나면 조건을 벗어난 시간을 줘서 2타 못하게함
		m_AtkCool_2 = true; // 3타 공격이 가능하게 만듬.
	}

	if (KEY_TAP(KEY::LBTN) && m_AtkCool_2 && m_Atk2Time >= 1.7f && m_Atk2Time <= 2.8f) // 3번째 공격 2 타 처럼 같은 시간을 씀
	{
		CreateAtk3_1();
		m_Atk2Time = 3.f; // 공격을 하고 나면 조건을 벗어난 시간을 줘서 2타 못하게함
		m_AtkCool_2 = false; // 3타 공격이 불가능해짐
	}
	
	if (m_Atk2Time > 3.f)
	{
		m_AtkCool_2 = false;
	}

}

void Player_Attack_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Attack_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Attack_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Player_Attack_Script::CreateAtk1()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();
	pPos += MousDir * m_AtkRange;

	// atan2를 사용하여 각도 계산 (왼손 좌표계)
	float angle = atan2(MousDir.y, MousDir.x);

	// 기본 각도 보정 (약 30도 = π/6 라디안)
	float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

	float baseScaleX = 350.0f;
	float baseScaleY = 350.0f;

	CGameObject* PlayerAtk1 = new CGameObject;
	PlayerAtk1->AddComponent(new CMeshRender);
	PlayerAtk1->AddComponent(new CCollider2D);
	PlayerAtk1->AddComponent(new CFlipbookPlayer);
	PlayerAtk1->AddComponent(new Player_Atk_Obj);
	PlayerAtk1->SetName(L"pAtk1");

	PlayerAtk1->Transform()->SetRelativePos(Vec3(pPos.x, pPos.y + 20.f, pPos.z));
	PlayerAtk1->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	PlayerAtk1->Transform()->SetIndependentScale(true);
	PlayerAtk1->Transform()->SetRelativeRotation(Vec3((XM_PI / 3), -(XM_PI / 3), correctedAngle));
	PlayerAtk1->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	PlayerAtk1->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	PlayerAtk1->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
	PlayerAtk1->FlipbookPlayer()->Play(0, 160.f, false);
	//PlayerAtk1->Collider2D()->SetIndependentScale(false);
	//PlayerAtk1->Collider2D()->SetScale(Vec2(1.f, 1.f));
	CreateObject(PlayerAtk1, 4, false);


}

void Player_Attack_Script::CreateAtk2()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();
	pPos += MousDir * m_AtkRange;

	// atan2를 사용하여 각도 계산 (왼손 좌표계)
	float angle = atan2(MousDir.y, MousDir.x);

	// 기본 각도 보정 (약 30도 = π/6 라디안)
	float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

	float baseScaleX = 300.0f;
	float baseScaleY = 300.0f;

	CGameObject* PlayerAtk2 = new CGameObject;
	PlayerAtk2->AddComponent(new CMeshRender);
	PlayerAtk2->AddComponent(new CCollider2D);
	PlayerAtk2->AddComponent(new CFlipbookPlayer);
	PlayerAtk2->AddComponent(new Player_Atk_Obj);
	PlayerAtk2->SetName(L"pAtk2");

	PlayerAtk2->Transform()->SetRelativePos(pPos);
	PlayerAtk2->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	PlayerAtk2->Transform()->SetIndependentScale(true);
	PlayerAtk2->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, correctedAngle));
	//PlayerAtk2->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, correctedAngle + (XM_PI / 2)));
	PlayerAtk2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	PlayerAtk2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	PlayerAtk2->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
	PlayerAtk2->FlipbookPlayer()->Play(0, 130.f, false);
	//PlayerAtk2->Collider2D()->SetIndependentScale(false);
	//PlayerAtk2->Collider2D()->SetScale(Vec2(1.f, 1.f));
	CreateObject(PlayerAtk2, 4, false);

}

void Player_Attack_Script::CreateAtk3_1()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();
	pPos += MousDir * m_AtkRange;

	// atan2를 사용하여 각도 계산 (왼손 좌표계)
	float angle = atan2(MousDir.y, MousDir.x);

	// 기본 각도 보정 (약 30도 = π/6 라디안)
	float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

	float baseScaleX = 300.0f;
	float baseScaleY = 300.0f;

	CGameObject* PlayerAtk1 = new CGameObject;
	PlayerAtk1->AddComponent(new CMeshRender);
	PlayerAtk1->AddComponent(new CCollider2D);
	PlayerAtk1->AddComponent(new CFlipbookPlayer);
	PlayerAtk1->AddComponent(new Player_Atk_Obj);
	PlayerAtk1->SetName(L"pAtk3_1");
	PlayerAtk1->Transform()->SetRelativePos(Vec3(pPos.x , pPos.y, pPos.z));
	PlayerAtk1->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	PlayerAtk1->Transform()->SetIndependentScale(true);
	PlayerAtk1->Transform()->SetRelativeRotation(Vec3((XM_PI/4), 0, correctedAngle - (XM_PI/2.3f)));
	PlayerAtk1->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	PlayerAtk1->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	PlayerAtk1->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
	PlayerAtk1->FlipbookPlayer()->Play(0, 100.f, false);
	//PlayerAtk1->Collider2D()->SetIndependentScale(false);
	//PlayerAtk1->Collider2D()->SetScale(Vec2(1.f, 1.f));
	CreateObject(PlayerAtk1, 4, false);

	// 플레이어 어택 오브젝트에게 델리게이트 호출시 발동할 함수 알려주기 
	Player_Atk_Obj* PlayerAtk = (Player_Atk_Obj*)PlayerAtk1->GetScript(L"Player_Atk_Obj");
	PlayerAtk->AddAtkObjDelegate(this, (SCR_DELEGATE)&Player_Attack_Script::CreateAtk3_2);

}

void Player_Attack_Script::CreateAtk3_2()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();
	pPos += MousDir * m_AtkRange;

	// atan2를 사용하여 각도 계산 (왼손 좌표계)
	float angle = atan2(MousDir.y, MousDir.x);

	// 기본 각도 보정 (약 30도 = π/6 라디안)
	float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

	float baseScaleX = 300.0f;
	float baseScaleY = 300.0f;

	CGameObject* PlayerAtk2 = new CGameObject;
	PlayerAtk2->AddComponent(new CMeshRender);
	PlayerAtk2->AddComponent(new CCollider2D);
	PlayerAtk2->AddComponent(new CFlipbookPlayer);
	PlayerAtk2->AddComponent(new Player_Atk_Obj);
	PlayerAtk2->SetName(L"pAtk3_2");
	PlayerAtk2->Transform()->SetRelativePos(pPos);
	PlayerAtk2->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	PlayerAtk2->Transform()->SetIndependentScale(true);
	PlayerAtk2->Transform()->SetRelativeRotation(Vec3((XM_PI * 1.3f), XM_PI / 5, correctedAngle - XM_PI * 1.3f));
	PlayerAtk2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	PlayerAtk2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	PlayerAtk2->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
	PlayerAtk2->FlipbookPlayer()->Play(0, 100.f, false);
	//PlayerAtk2->Collider2D()->SetIndependentScale(false);
	//PlayerAtk2->Collider2D()->SetScale(Vec2(1.f, 1.f));
	CreateObject(PlayerAtk2, 4, false);
}


// 첫타 60 0, 이타 60, -160
// 첫타 반전하려면 Z값에 + 180도 

// 3타 x자 교체 공격 	이렇게 해주니까 x 모양 됌 
// float PI = XM_PI / 10;
//Rota.x += PI;
//Rota.y += PI;
//Rota.z += PI;