#include "pch.h"
#include "CBoss_Dash.h"
#include "CBoss_Ai.h"

CBoss_Dash::CBoss_Dash()
	: CScript((UINT)SCRIPT_TYPE::BOSS_DASH)
    , m_Dash_Obj(nullptr)
    , m_fDashDuration(0.f)
    , m_fCurrentTime(0.f)
    , m_vStartPos(0.f, 0.f, 0.f)
    , m_vTargetPos(0.f,0.f,0.f)
{
}

CBoss_Dash::~CBoss_Dash()
{
}


void CBoss_Dash::Begin()
{
	SetAI(true);
    m_Dash_Obj = nullptr;
    m_fDashDuration = 0.f;
    m_fCurrentTime = 0.f;
    m_vStartPos= Vec3(0.f, 0.f, 0.f);
    m_vTargetPos = Vec3(0.f, 0.f, 0.f);

}

void CBoss_Dash::AI_In()
{
    m_Dash_Obj = nullptr;

	CBoss_Ai* TEMP_AI = (CBoss_Ai*)GetOwner()->GetScript((UINT)SCRIPT_TYPE::BOSS_AI);
	Vec3 TargetPos = TEMP_AI->GetTargetPlayer()->Transform()->GetWorldPos();
	m_vStartPos = Transform()->GetWorldPos();
	m_vTargetPos = TEMP_AI->GetTargetPlayer()->Transform()->GetWorldPos();

	m_fDashDuration = 0.3f;  // 1초로 설정
	m_fCurrentTime = 0.f;

    CreateAtk();
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Dash, 13.f, false);
}

void CBoss_Dash::AI_Tick()
{
    m_fCurrentTime += DT;
    float t = m_fCurrentTime / m_fDashDuration;  // 0~1 사이의 진행률

    if (t >= 0.9f)
    {
        Transform()->SetRelativePos(m_vTargetPos);
    }
    else
    {
        // 선형 보간으로 위치 계산
        Vec3 newPos = Vec3::Lerp(m_vStartPos, m_vTargetPos, t);
        Transform()->SetRelativePos(newPos);
    }

    if (FlipbookPlayer()->isFinish())
    {
        B_CHANGE(SCRIPT_TYPE::BOSS_NOR_ATK1);
        return;
    }
}


void CBoss_Dash::AI_Exit()
{
    if(m_Dash_Obj != nullptr)
        DestroyObject(m_Dash_Obj);
}

void CBoss_Dash::CreateAtk()
{
    Vec3 Pos(0.f, 0.f, 0.f);
    float RotaZ = XM_PI / 6;

    if (m_vStartPos.x < m_vTargetPos.x)
    {
        Pos = Vec3(-30.f, -90.f, 11.f);
        RotaZ *= -1.f;
    }
    else
    {
        Pos = Vec3(30.f, -90.f, 11.f);
    }


    m_Dash_Obj = new CGameObject;
    m_Dash_Obj->SetName(L"Dash_Obj");

    m_Dash_Obj->AddComponent(new CRepeatSprite);
    m_Dash_Obj->RepeatSprite()->SetSingleImageSize(120.f, 200.f);
    m_Dash_Obj->RepeatSprite()->SetRepeatCount(2, 1);
    m_Dash_Obj->RepeatSprite()->SetTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\JetFire.png"));
    m_Dash_Obj->RepeatSprite()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, 0.5f);

    m_Dash_Obj->Transform()->SetIndependentScale(true);
    m_Dash_Obj->Transform()->SetRelativePos(Pos);
    m_Dash_Obj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, RotaZ));

    CreateObject(m_Dash_Obj, 0, false);
    AddChild(GetOwner(), m_Dash_Obj);

}

void CBoss_Dash::SaveComponent(FILE* _File)
{
}

void CBoss_Dash::LoadComponent(FILE* _File)
{
}

void CBoss_Dash::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Dash::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Dash::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


