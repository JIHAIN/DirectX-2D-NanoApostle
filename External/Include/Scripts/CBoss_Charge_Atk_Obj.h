#pragma once
#include <Engine/CScript.h>

class CBoss_Charge_Atk_Obj :
    public CScript
{
private:
    Vec3        m_Velocity;
    float       m_DeleteTime;
    float       m_ScaleTime;
    bool        m_smaller;

    Vec3        m_Originalsize;

    float        AccTime;
    CGameObject* m_TargetPlayer;
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override {};
    virtual void LoadComponent(FILE* _File) override {};
public:
    CLONE(CBoss_Charge_Atk_Obj);
    CBoss_Charge_Atk_Obj();
    ~CBoss_Charge_Atk_Obj();
};

