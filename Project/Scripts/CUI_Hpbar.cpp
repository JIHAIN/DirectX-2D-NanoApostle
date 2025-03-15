#include "pch.h"
#include "CUI_Hpbar.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayer_Ai.h"

CUI_Hpbar::CUI_Hpbar()
    : CScript((UINT)SCRIPT_TYPE::UI_HPBAR)
    , m_MaxHP(100)
    , m_CurrentHP(100)
    , m_TargetObject(nullptr) // HP를 표시할 대상 오브젝트
    , m_OriginalPos(0.f, 0.f, 0.f)
    , m_OriginalScalex(0.f)
    , m_Death(false)
{
}

CUI_Hpbar::~CUI_Hpbar()
{
}

void CUI_Hpbar::SaveComponent(FILE* _File)
{
}

void CUI_Hpbar::LoadComponent(FILE* _File)
{
}

void CUI_Hpbar::Begin()
{
    m_MaxHP = 100.f;
    m_CurrentHP = m_MaxHP;
    m_OriginalPos = Transform()->GetRelativePos();  // 초기 위치 저장
    m_OriginalScalex = Transform()->GetRelativeScale().x;
    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    m_TargetObject = CurLevel->FindObjectByName(L"Player");
    m_Death = false;
}

void CUI_Hpbar::Tick()
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
           CPlayer_Ai* PlayerAi = (CPlayer_Ai*)m_TargetObject->GetScript(SCRIPT_TYPE::PLAYER_AI);
           PlayerAi->ChangeAi(SCRIPT_TYPE::PLAYER_DEATH);


       }
    }

    
}