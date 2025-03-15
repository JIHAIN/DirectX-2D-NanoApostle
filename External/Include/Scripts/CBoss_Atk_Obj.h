#pragma once
#include <Engine/CScript.h>

class CBoss_Atk_Obj :
    public CScript
{
private:
    bool        m_firstEnd;
    float       m_Time;
    wstring     m_Name;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CBoss_Atk_Obj);
    CBoss_Atk_Obj();
    ~CBoss_Atk_Obj();
};