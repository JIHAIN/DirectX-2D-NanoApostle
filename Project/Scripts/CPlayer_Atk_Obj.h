#pragma once
#include <Engine/CScript.h>

class CPlayer_Atk_Obj :
    public CScript
{
private:
    bool        m_SpriteRotfrist;

    wstring     m_Name;

    // 델리게이트 용
    CScript*        m_DlgtInst;
    SCR_DELEGATE    m_DlgtMemFunc;

    CGameObject* pParticle;
    CGameObject* m_EffectDodge;
    CGameObject* m_EffectBlood;


public:
    tParticleModule ParticleSetting();

    tParticleModule ParticleSetting1(Vec3 _Dir);

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    void AddAtkObjDelegate(CScript* _Inst, SCR_DELEGATE _MemFunc)
    {
        m_DlgtInst = _Inst;
        m_DlgtMemFunc = _MemFunc;
    }
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CPlayer_Atk_Obj);

    CPlayer_Atk_Obj();
    ~CPlayer_Atk_Obj();
};