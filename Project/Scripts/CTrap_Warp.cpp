#include "pch.h"
#include "CTrap_Warp.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CTrap_Warp::CTrap_Warp()
    : CScript((UINT)SCRIPT_TYPE::TRAP_WARP)
    , m_Time(0.f)
    , m_WarpOn(false)
    , PostObj(nullptr)
{
}

CTrap_Warp::~CTrap_Warp()
{
    if (PostObj != nullptr)
        DestroyObject(PostObj);
}


void CTrap_Warp::Tick()
{
    if (m_WarpOn)
    {
        m_Time += DT;

        // 진행도 계산 (0 ~ 1)
        float progress = m_Time / 2.f;

        // 포스트 프로세스 효과에 진행도 전달
        if (PostObj && PostObj->MeshRender())
        {
            PostObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, progress);
        }
        if (m_Time >= 2.f)
        {
            wstring CurNmae = L"Main";
            CLevelMgr::GetInst()->RequestLevelLoad(CurNmae);
        }
    }
}

void CTrap_Warp::Begin()
{
    m_Time = 0.f;
    m_WarpOn = false;
}

void CTrap_Warp::createPost()
{
    PostObj = new CGameObject;
    PostObj->SetName(L"FadeOut");
    PostObj->AddComponent(new CMeshRender);

    PostObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
    PostObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    PostObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"FadeOutEffectMtrl"));
    PostObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
    CreateObject(PostObj, 4, false);
}

void CTrap_Warp::SaveComponent(FILE* _File)
{
}

void CTrap_Warp::LoadComponent(FILE* _File)
{
}

void CTrap_Warp::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetName() == L"Player")
    {
        m_WarpOn = true;
    }
}

void CTrap_Warp::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CTrap_Warp::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}



