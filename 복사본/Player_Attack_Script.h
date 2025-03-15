#pragma once
#include <Engine/CScript.h>

class Player_Attack_Script :
    public CScript
{
private:
    int     m_AtkDmg;
    float   m_AtkRange;
    float   m_AtkTime;
    float   m_Atk2Time;
    bool    m_AtkCool;
    bool    m_AtkCool_2;

    Vec3    MousDir;
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void CreateAtk1();
    void CreateAtk2();
    void CreateAtk3_1();
    void CreateAtk3_2();


public:
    CLONE_DISABLE(Player_Attack_Script);

    Player_Attack_Script();
    ~Player_Attack_Script();
};

