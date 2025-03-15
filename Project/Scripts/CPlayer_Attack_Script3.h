#pragma once
#include <Engine/CScript.h>

class CPlayer_Attack_Script3 :
    public CScript
{
private:
    int             m_AtkDmg;
    float           m_AtkRange;
    Vec3            MousDir;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void CreateAtk3_1();
    void CreateAtk3_2();
    void PlayerAniPlay3(Vec3 _Dir);

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;


public:
    CLONE(CPlayer_Attack_Script3);

    CPlayer_Attack_Script3();
    ~CPlayer_Attack_Script3();
};

