#pragma once
#include <Engine/CScript.h>

class CPlayer_Hook_Script :
    public CScript
{
private:
    int     AtkDmg;
    Vec3    m_MyPos;         // ���� ��ġ
    CGameObject*    m_Monster;   // ��ǥ
    CGameObject*    m_Chain;    // ü��
    float   m_fDashDuration;    // ��� ���ӽð�
    float   m_fCurrentTime;     // ���� ����� �ð�

public:
    virtual void Begin() override;
    virtual void Tick() {};
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    void PlayerAniPlay1(Vec3 _Dir);

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE(CPlayer_Hook_Script);
    CPlayer_Hook_Script();
    ~CPlayer_Hook_Script();
};

