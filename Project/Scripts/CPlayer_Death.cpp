#include "pch.h"
#include "CPlayer_Death.h"
#include "CPlayer_Ai.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>


CPlayer_Death::CPlayer_Death()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_DEATH)
	, m_DeathPost(nullptr)
{
}

CPlayer_Death::~CPlayer_Death()
{
}

void CPlayer_Death::Begin()
{
	m_DeathPost = nullptr;
}

void CPlayer_Death::SaveComponent(FILE* _File)
{
}

void CPlayer_Death::LoadComponent(FILE* _File)
{
}

void CPlayer_Death::AI_In()
{
	FlipbookPlayer()->Play((UINT)Player_Ani_State::Death, 6.f, false);
	CreatePost();
}

void CPlayer_Death::AI_Tick()
{
    // �ִϸ��̼��� ���� ���൵�� 0~1 ���� ������ ���
    float progress = FlipbookPlayer()->GetAnimationProgress();

    // PostEffect ��Ƽ������ g_float_0 �� ������Ʈ
    if (m_DeathPost != nullptr)
    {
        m_DeathPost->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, progress);
    }

    if (FlipbookPlayer()->isFinish())
    {
        wstring CurNmae = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
        CLevelMgr::GetInst()->RequestLevelLoad(CurNmae);
    }
}

void CPlayer_Death::AI_Exit()
{
}

void CPlayer_Death::CreatePost()
{
	m_DeathPost = new CGameObject;
	m_DeathPost->AddComponent(new CMeshRender);

	m_DeathPost->MeshRender()->SetMesh(L"RectMesh");
	m_DeathPost->MeshRender()->SetMaterial(L"GameOverMtrl");

	CreateObject(m_DeathPost, 4, false);
	
}

