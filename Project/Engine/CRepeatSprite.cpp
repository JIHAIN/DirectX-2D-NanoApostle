// CRepeatSprite.cpp
#include "pch.h"
#include "CRepeatSprite.h"
#include "CTransform.h"
#include "CAssetMgr.h"
#include "CDevice.h"
#include "CLayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"


CRepeatSprite::CRepeatSprite()
    : CRenderComponent(COMPONENT_TYPE::REPEATSPRITE)
    , m_RepeatCount(1.f, 1.f)
    , m_SingleImageSize(100.f, 100.f)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RepeatSpriteMtrl"));
}

CRepeatSprite::~CRepeatSprite()
{
}

void CRepeatSprite::UpdateScale()
{
    Transform()->SetRelativeScale(Vec3(m_SingleImageSize.x * m_RepeatCount.x,
        m_SingleImageSize.y * m_RepeatCount.y,
        1.f));
}

void CRepeatSprite::Init()
{
    UpdateScale();
}

void CRepeatSprite::Render()
{
    if (nullptr == m_Texture)
        return;

    Transform()->Binding();

    GetMaterial()->SetTexParam(TEX_0, m_Texture);
    GetMaterial()->SetScalarParam(VEC2_0, m_RepeatCount);
    GetMaterial()->Binding();

    GetMesh()->Render();
}

void CRepeatSprite::SaveComponent(FILE* _File)
{
    fwrite(&m_RepeatCount, sizeof(Vec2), 1, _File);
    fwrite(&m_SingleImageSize, sizeof(Vec2), 1, _File);
    SaveAssetRef(m_Texture, _File);
}

void CRepeatSprite::LoadComponent(FILE* _File)
{
    fread(&m_RepeatCount, sizeof(Vec2), 1, _File);
    fread(&m_SingleImageSize, sizeof(Vec2), 1, _File);
    LoadAssetRef(m_Texture, _File);

    UpdateScale();
}
