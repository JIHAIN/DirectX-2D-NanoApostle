#pragma once
#include <Engine/CScript.h>

class CPlayer_Arrow_Script :
    public CScript
{
private:
    Vec3            MousDir;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void AI_In() override;
    virtual void AI_Tick() override;
    virtual void AI_Exit() override;

    void PlayerAniPlay1(Vec3 _Dir);


public:
    void CreateArrow();



public:
    CLONE(CPlayer_Arrow_Script);
    CPlayer_Arrow_Script();
    ~CPlayer_Arrow_Script();
};

