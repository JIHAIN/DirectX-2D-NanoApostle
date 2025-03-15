#include "pch.h"
#include "CBoss_Roar.h"
#include "CBoss_Ai.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CCameraScript.h"

CBoss_Roar::CBoss_Roar()
	: CScript((UINT)SCRIPT_TYPE::BOSS_ROAR)
	, m_first(true)
	, m_Time(0.f)
	, m_AniStart(false)
{
}

CBoss_Roar::~CBoss_Roar()
{
}


void CBoss_Roar::Begin()
{
	SetAI(true);
	m_Time = 0.f;
	m_AniStart = false;
}

void CBoss_Roar::AI_In()
{
	m_first = true; // 로어 한번만 할 수 있게하는 bool값
	m_Time = 0.f;
	m_AniStart = false;


}

void CBoss_Roar::AI_Tick()
{
	m_Time += DT;

	if (m_Time < 2.f)
		return;
	else if (m_Time > 2.f && !m_AniStart)
	{
		m_AniStart = true;

		FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Charge_Atk, 13.f, false);
		FlipbookPlayer()->SetTerm(6, 2.f);

		CGameObject* Cam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MainCamera");
		CCameraScript* pCamScript = (CCameraScript*)Cam->GetScript((UINT)SCRIPT_TYPE::CAMERASCRIPT);
		pCamScript->SetTemporaryTarget(GetOwner(), 2.50f, 3.0f);
	}



	if ( FlipbookPlayer()->GetCurIdx() == 5 && m_first)
	{
		m_first = false;

		pPostProcess = new CGameObject;
		pPostProcess->SetName(L"PostRoar");
		pPostProcess->AddComponent(new CMeshRender);

		pPostProcess->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		pPostProcess->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pPostProcess->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RoarMtrl"));
		pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
		pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"pRoar"));

		CreateObject(pPostProcess, 4, false);

		// Sound 로딩하기
		Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BossRoar.wav", L"Sound\\BossRoar.wav");
		pSound->Play(1, 0.3f, true);
	}


	if (FlipbookPlayer()->isFinish())
	{
		if(pPostProcess != nullptr)
			DestroyObject(pPostProcess);

		B_CHANGE(SCRIPT_TYPE::BOSS_IDLE);
		return;
	}
}

void CBoss_Roar::AI_Exit()
{

}


void CBoss_Roar::SaveComponent(FILE* _File)
{
}

void CBoss_Roar::LoadComponent(FILE* _File)
{
}


