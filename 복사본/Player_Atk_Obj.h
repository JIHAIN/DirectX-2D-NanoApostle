#pragma once
#include <Engine/CScript.h>

class Player_Atk_Obj :
    public CScript
{
private:
    float       m_DeleteTime;
    Vec3        m_OriginRot;
    bool        m_SpriteRotfrist;

    wstring     m_Name;

    // 델리게이트 용
    CScript*        m_DlgtInst;
    SCR_DELEGATE    m_DlgtMemFunc;

public:


public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    void AddAtkObjDelegate(CScript* _Inst, SCR_DELEGATE _MemFunc)
    {
        m_DlgtInst = _Inst;
        m_DlgtMemFunc = _MemFunc;
    }

public:
    CLONE_DISABLE(Player_Atk_Obj);

    Player_Atk_Obj();
    ~Player_Atk_Obj();
};