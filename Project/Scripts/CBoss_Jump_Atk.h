#pragma once
#include <Engine/CScript.h>

class CBoss_Jump_Atk :
    public CScript
{
private:
    float       m_fCurrentTime;     // 현재 진행된 시간
    Vec3        m_vStartPos;        // 시작 위치
    Vec3        m_vTargetPos;       // 목표 위치
    float       m_JumpHeight;       // 최대 점프 높이
    float       m_HoverTime;        // 공중 체공 시간
    float       m_CurrentHoverTime; // 현재 체공 시간
    enum class JumpPhase
    {
        RISING,     // 상승
        HOVERING,   // 체공
        FALLING     // 낙하
    };
    JumpPhase    m_Phase;
    CGameObject* m_Jump_Obj;
    CGameObject* pParticle;
    CGameObject* pParticle1;
    CGameObject* pParticle2;

public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void AI_In() override;
    virtual void AI_Tick() override;
    virtual void AI_Exit() override;

    tParticleModule ParticleSetting();
    tParticleModule ParticleSetting1();
    tParticleModule ParticleSetting2();
    void CreateOnject();
public:
    CLONE(CBoss_Jump_Atk);
    CBoss_Jump_Atk();
    ~CBoss_Jump_Atk();
};
