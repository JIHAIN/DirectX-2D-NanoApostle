#include "pch.h"
#include "CBoss_Bottom_Atk.h"
#include "CBoss_Ai.h"
#include "CBoss_Atk_Obj.h"

CBoss_Bottom_Atk::CBoss_Bottom_Atk()
	: CScript((UINT)SCRIPT_TYPE::BOSS_BOTTOM_ATK)
	, m_Atk(false)
{
}

CBoss_Bottom_Atk::~CBoss_Bottom_Atk()
{
}


void CBoss_Bottom_Atk::Begin()
{
	SetAI(true);
	m_Atk = false;
}

void CBoss_Bottom_Atk::AI_In()
{

	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Bottom_Atk, 13.f, false);
	m_Atk = false;

}

void CBoss_Bottom_Atk::AI_Tick()
{
	if (FlipbookPlayer()->GetCurIdx() == 7 && !m_Atk)
	{
		CreateAtk();
		m_Atk = true;
	}

	if (FlipbookPlayer()->isFinish())
	{
		// 0 또는 1의 랜덤값 생성
		int randomValue = rand() % 2;

		if (randomValue == 0)
		{
			B_CHANGE(SCRIPT_TYPE::BOSS_IDLE);
		}
		else
		{
			B_CHANGE(SCRIPT_TYPE::BOSS_DASH);
		}		return;
	}
}



void CBoss_Bottom_Atk::AI_Exit()
{
}


void CBoss_Bottom_Atk::SaveComponent(FILE* _File)
{
}

void CBoss_Bottom_Atk::LoadComponent(FILE* _File)
{
}

void CBoss_Bottom_Atk::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Bottom_Atk::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Bottom_Atk::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Bottom_Atk::CreateAtk()
{
	float baseScaleX = 0.0f;
	float baseScaleY = 0.0f;
	Vec3 Rota = Vec3(0.f, 0.f, 0.f);

	Vec3 Pos = GetOwner()->Transform()->GetRelativePos();

	bool left = CBoss_Ai::IsLeft(); // 보스가 왼쪽 보는지 오른쪽 보는지
	float Angle = XM_PI / 18; // 10도

	CGameObject* BossAtk = new CGameObject;
	BossAtk->AddComponent(new CMeshRender);
	BossAtk->AddComponent(new CCollider2D);
	BossAtk->AddComponent(new CFlipbookPlayer);
	BossAtk->AddComponent(new CBoss_Atk_Obj);
	BossAtk->SetName(L"B_Atk3");

	BossAtk->AddComponent(new CLight2D);
	BossAtk->Light2D()->SetLightColor(Vec3(0.1f, 0.1f, 0.1f));
	BossAtk->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	BossAtk->Light2D()->SetRadius(50.f);

	BossAtk->Collider2D()->SetIndependentScale(false);
	BossAtk->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

	BossAtk->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	BossAtk->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossSkillMtrl"));
	BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj_First"));
	BossAtk->FlipbookPlayer()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj2"));
	BossAtk->Transform()->SetIndependentScale(true);

	if (left) // 보스가 왼쪽을 보고있다면
	{
		// 왼쪽
		Pos.x -= 40.f;
		Pos.y -= 20.f;
		baseScaleX = -500.0f;
		baseScaleY = 600.0f;
		Rota = Vec3(Angle * -5.f, Angle * -4.f, 0.f);
	}
	else // 오른쪽
	{
		Pos.x += 40.f;
		Pos.y -= 20.f;
		baseScaleX = 500.0f;
		baseScaleY = 600.0f;

		Rota = Vec3(Angle * -5.f, Angle * 4.f, 0.f);
	}
	BossAtk->Transform()->SetRelativePos(Pos);
	BossAtk->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	BossAtk->Transform()->SetRelativeRotation(Rota);
	BossAtk->FlipbookPlayer()->Play(0, 80.f, false);
	CreateObject(BossAtk, 6, false);

	BossAtk = nullptr;

	BossAtk = new CGameObject;
	BossAtk->AddComponent(new CMeshRender);
	BossAtk->AddComponent(new CCollider2D);
	BossAtk->AddComponent(new CFlipbookPlayer);
	BossAtk->AddComponent(new CBoss_Atk_Obj);
	BossAtk->SetName(L"B_Atk3");

	BossAtk->AddComponent(new CLight2D);
	BossAtk->Light2D()->SetLightColor(Vec3(0.1f, 0.1f, 0.1f));
	BossAtk->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	BossAtk->Light2D()->SetRadius(50.f);

	BossAtk->Collider2D()->SetIndependentScale(false);
	BossAtk->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

	BossAtk->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	BossAtk->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossSkillMtrl"));
	BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj_First"));
	BossAtk->FlipbookPlayer()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj2"));
	BossAtk->Transform()->SetIndependentScale(true);


	if (left) // 보스가 왼쪽을 보고있다면
	{
		Pos = GetOwner()->Transform()->GetRelativePos();
		Pos.x -= 30.f;
		Pos.y -= 11.f;
		baseScaleX = 550.0f;
		baseScaleY = 500.0f;
		Rota = Vec3(0.f, XM_PI, Angle);
	}
	else
	{
		Pos = GetOwner()->Transform()->GetRelativePos();
		Pos.x -= 30.f;
		Pos.y -= 11.f;
		baseScaleX = 550.0f;
		baseScaleY = 500.0f;
		Rota = Vec3(0.f, 0.f, Angle);
	}


	BossAtk->Transform()->SetRelativePos(Pos);
	BossAtk->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	BossAtk->Transform()->SetRelativeRotation(Rota);
	BossAtk->FlipbookPlayer()->Play(0, 80.f, false);

	CreateObject(BossAtk, 6, false);

}


