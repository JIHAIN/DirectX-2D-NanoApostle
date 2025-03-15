#include "pch.h"
#include "CBoss_Dead.h"
#include "CBoss_Ai.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CCameraScript.h"

CBoss_Dead::CBoss_Dead()
	: CScript((UINT)SCRIPT_TYPE::BOSS_DEAD)
	, m_Excute_Obj(nullptr)
	, m_Destroy(false)
{
}

CBoss_Dead::~CBoss_Dead()
{
}


void CBoss_Dead::Begin()
{
	SetAI(true);
	m_Excute_Obj = nullptr;
	m_Destroy = false;
}

void CBoss_Dead::AI_In()
{
	GetOwner()->Transform()->SetRelativeScale(Vec3(750.f, 750.f, 1.f));
	GetOwner()->Collider2D()->SetOffset(Vec2(0.f, -0.4f));
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Execute, 6.f, false);
	CreateAtk();
	m_Destroy = false;

	CGameObject* Cam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MainCamera");
	CCameraScript* pCamScript = (CCameraScript*)Cam->GetScript((UINT)SCRIPT_TYPE::CAMERASCRIPT);
	pCamScript->SetTemporaryTarget(GetOwner(), 14.f, 1.0f);
}

void CBoss_Dead::AI_Tick()
{
	if (!m_Destroy && m_Excute_Obj->FlipbookPlayer()->isFinish())
	{
		m_Destroy = true;

		DestroyObject(m_Excute_Obj);
	}



	//if (FlipbookPlayer()->isFinish())
	//{
	//	B_CHANGE(SCRIPT_TYPE::BOSS_IDLE);
	//	return;
	//}
}

void CBoss_Dead::AI_Exit()
{
	
}

void CBoss_Dead::CreateAtk()
{
	Vec3 Pos = Transform()->GetRelativePos();
	Pos.z = 9.f;

	m_Excute_Obj = new CGameObject;

	m_Excute_Obj->AddComponent(new CMeshRender);
	m_Excute_Obj->AddComponent(new CFlipbookPlayer);
	m_Excute_Obj->SetName(L"Excute");
	
	m_Excute_Obj->Transform()->SetRelativePos(Pos);
	m_Excute_Obj->Transform()->SetRelativeScale(Vec3(650.f, 650.f, 1.f));
	m_Excute_Obj->Transform()->SetIndependentScale(true);
	m_Excute_Obj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	m_Excute_Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	m_Excute_Obj->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Execute_PlayerTentacle"));
	m_Excute_Obj->FlipbookPlayer()->Play(0, 6.f, false);
	CreateObject(m_Excute_Obj, 0, false);

	Pos.x -= 25.f;
	Pos.y += 100.f;
	Pos.z = 11.f;

	Transform()->SetRelativePos(Pos);
}


void CBoss_Dead::SaveComponent(FILE* _File)
{
}

void CBoss_Dead::LoadComponent(FILE* _File)
{
}


