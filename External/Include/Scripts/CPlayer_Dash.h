#pragma once
#include <Engine/CScript.h>

enum class CPlayer_Dash_State;
class CPlayerScript;

class CPlayer_Dash :
    public CScript
{
private:
    CPlayerScript*  PlayerScript;
    CGameObject*    pParticle;
    

    // 대시 관련 상수
    const float     DASH_DISTANCE = 250.f;     // 고정된 대시 거리
    const float     DASH_DURATION = 0.3f;      // 대시 지속 시간

    // 대시 상태 변수
    Vec2            m_DashStartPos;            // 대시 시작 위치
    Vec2            m_DashTargetPos;           // 대시 목표 위치
    Vec2            m_DashDirection;           // 대시 방향
    float           m_DashProgress;            // 대시 진행도 (0~1)
    float           m_CooldownTimer;           // 쿨다운 타이머
    bool            m_IsDashing;               // 대시 중인지 여부

    tParticleModule ParticleSetting(Vec3 _Dir);
    
public:
    Vec2 Lerp(const Vec2& start, const Vec2& end, float t);


    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE_DISABLE(CPlayer_Dash);
    CPlayer_Dash();
    ~CPlayer_Dash();
};


