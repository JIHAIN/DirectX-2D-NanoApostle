#pragma once
#include <Engine/CScript.h>

class CPlayerScript;

class CPlayer_Attack_Script1 :
    public CScript
{
private:
    int             m_AtkDmg;
    float           m_AtkRange;
    float           m_AtkTime;
    Vec3            MousDir;
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void CreateAtk1();
    void PlayerAniPlay1(Vec3 _Dir);


    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;


public:
    CLONE(CPlayer_Attack_Script1);

    CPlayer_Attack_Script1();
    ~CPlayer_Attack_Script1();
};

