#pragma once
#include <Engine/CScript.h>

class CBoss_Jump_Atk :
    public CScript
{
private:
    float       m_fCurrentTime;     // ���� ����� �ð�
    Vec3        m_vStartPos;        // ���� ��ġ
    Vec3        m_vTargetPos;       // ��ǥ ��ġ
    float       m_JumpHeight;       // �ִ� ���� ����
    float       m_HoverTime;        // ���� ü�� �ð�
    float       m_CurrentHoverTime; // ���� ü�� �ð�
    enum class JumpPhase
    {
        RISING,     // ���
        HOVERING,   // ü��
        FALLING     // ����
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
