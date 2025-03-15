#pragma once
#include <Engine/CScript.h>

class CWallScript;

class CWallOpen :
    public CScript
{
private:
    float        m_OpenTime;
    CWallScript* m_ParentScript;
    CGameObject* m_ParentObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override {};
    virtual void LoadComponent(FILE* _File) override {};
public:
    CLONE(CWallOpen);
    CWallOpen();
    ~CWallOpen();
};

