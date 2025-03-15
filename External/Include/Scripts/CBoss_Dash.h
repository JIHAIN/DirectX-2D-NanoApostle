#pragma once
#include <Engine/CScript.h>

class CBoss_Dash :
    public CScript
{
private:
    float        m_fDashDuration;    // ��� ���ӽð�
    float        m_fCurrentTime;     // ���� ����� �ð�
    Vec3         m_vStartPos;         // ���� ��ġ
    Vec3         m_vTargetPos;        // ��ǥ ��ġ

    CGameObject* m_Dash_Obj;
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

    void CreateAtk();

public:
    CLONE(CBoss_Dash);
    CBoss_Dash();
    ~CBoss_Dash();
};

