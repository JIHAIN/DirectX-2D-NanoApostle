#pragma once
#include <Engine/CScript.h>

class CBoss_Bottom_Atk :
    public CScript
{
private:
    bool m_Atk;
public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void CreateAtk();

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE(CBoss_Bottom_Atk);
    CBoss_Bottom_Atk();
    ~CBoss_Bottom_Atk();
};
