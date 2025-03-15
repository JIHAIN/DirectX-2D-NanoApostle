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
    

    // ��� ���� ���
    const float     DASH_DISTANCE = 250.f;     // ������ ��� �Ÿ�
    const float     DASH_DURATION = 0.3f;      // ��� ���� �ð�

    // ��� ���� ����
    Vec2            m_DashStartPos;            // ��� ���� ��ġ
    Vec2            m_DashTargetPos;           // ��� ��ǥ ��ġ
    Vec2            m_DashDirection;           // ��� ����
    float           m_DashProgress;            // ��� ���൵ (0~1)
    float           m_CooldownTimer;           // ��ٿ� Ÿ�̸�
    bool            m_IsDashing;               // ��� ������ ����

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


