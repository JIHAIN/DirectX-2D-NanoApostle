#include "pch.h"
#include "CUI_BossHP.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CBoss_Ai.h"

CUI_BossHP::CUI_BossHP()
    : CScript((UINT)SCRIPT_TYPE::UI_BOSSHP)
    , m_MaxHP(100)
    , m_CurrentHP(100)
    , m_TargetObject(nullptr) // HP를 표시할 대상 오브젝트
    , m_OriginalPos(0.f, 0.f, 0.f)
    , m_OriginalScalex(0.f)
    , m_Death(false)
{
}

CUI_BossHP::~CUI_BossHP()
{
}

void CUI_BossHP::SaveComponent(FILE* _File)
{
}

void CUI_BossHP::LoadComponent(FILE* _File)
{
}

void CUI_BossHP::Begin()
{
    m_MaxHP = 100.f;
    m_CurrentHP = m_MaxHP;
    m_OriginalPos = Transform()->GetRelativePos();  // 초기 위치 저장
    m_OriginalScalex = Transform()->GetRelativeScale().x;
    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    m_TargetObject = CurLevel->FindObjectByName(L"Boss");
    m_Death = false;
}

void CUI_BossHP::Tick()
{

    if (m_TargetObject && !m_Death)
    {
        if (m_CurrentHP <= 0)
        {
            m_CurrentHP = 0.f;
            m_Death = true;
        }

        float ratio = m_CurrentHP / m_MaxHP;

        Vec3 scale = Transform()->GetRelativeScale();
        scale.x = m_OriginalScalex * ratio;

        Vec3 newPos = m_OriginalPos;
        newPos.x = m_OriginalPos.x - (m_OriginalScalex - scale.x) / 2.f;

        Transform()->SetRelativeScale(scale);
        Transform()->SetRelativePos(newPos);

        if (m_Death)
        {
            CBoss_Ai* BossAi = (CBoss_Ai*)m_TargetObject->GetScript(SCRIPT_TYPE::BOSS_AI);
            BossAi->ChangeAi(SCRIPT_TYPE::BOSS_GROGGY);

            CGameObject* HP = GetOwner()->GetParent();
            DestroyObject(HP);
        }
    }


}