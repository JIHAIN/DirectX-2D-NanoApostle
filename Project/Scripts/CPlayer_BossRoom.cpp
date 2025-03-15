#include "pch.h"
#include "CPlayer_BossRoom.h"
#include "CPlayer_Ai.h"

CPlayer_BossRoom::CPlayer_BossRoom()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_BOSSROOM)
	, PostObj(nullptr)
	, m_Time(0.f)
{
}

CPlayer_BossRoom::~CPlayer_BossRoom()
{
}


void CPlayer_BossRoom::Begin()
{
    SetAI(true);
}

void CPlayer_BossRoom::SaveComponent(FILE* _File)
{
}

void CPlayer_BossRoom::LoadComponent(FILE* _File)
{
}

void CPlayer_BossRoom::createPost()
{
	PostObj = new CGameObject;
	PostObj->SetName(L"FadeOut");
	PostObj->AddComponent(new CMeshRender);

	PostObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
	PostObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	PostObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"FadeInEffectMtrl"));
	PostObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	CreateObject(PostObj, 4, false);
}

void CPlayer_BossRoom::AI_In()
{
	PostObj = nullptr;
	createPost();
	FlipbookPlayer()->Play((UINT)Player_Ani_State::appeared, 6.f, false);
	FlipbookPlayer()->SetTerm(0, 2.f);
	m_Time = 0.f;
}

void CPlayer_BossRoom::AI_Tick()
{

	m_Time += DT;

	// 진행도 계산 (0 ~ 1)
	float progress = m_Time / 3.f;

	// 포스트 프로세스 효과에 진행도 전달
	if (PostObj && PostObj->MeshRender())
	{
		PostObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, progress);
	}

    // 애니메이션이 끝나면 기본 상태로
    if (FlipbookPlayer()->isFinish())
    {
        P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
        return;
    }
}

void CPlayer_BossRoom::AI_Exit()
{
	if (PostObj != nullptr)
		DestroyObject(PostObj);
}

