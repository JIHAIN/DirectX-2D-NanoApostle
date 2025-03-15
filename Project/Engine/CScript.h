#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "components.h"

struct tScriptParam
{
    SCRIPT_PARAM    Param;
    string          Desc;
    void* pData;
};


class CScript :
    public CComponent
{
private:
    const UINT  m_ScriptType;
    vector<tScriptParam>    m_vecScriptParam;

    bool m_AI; // AI ��ũ��Ʈ���� Ȯ��

public:
    UINT GetScriptType() { return m_ScriptType; }
    void AddScriptParam(tScriptParam _Param) { m_vecScriptParam.push_back(_Param); }
    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }
    void Instantiate(Ptr<CPrefab> _Pref, Vec3 _WorldPos, int _Layer);

public:
    virtual void Tick() = 0;
    virtual void FinalTick() final {}

    virtual void BeginOverlap(class CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {};
    virtual void Overlap(class CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {};
    virtual void EndOverlap(class CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {};

    void SetAI(bool _AI) { m_AI = _AI; }
    bool IsAI() { return m_AI; }
  

    virtual void AI_Tick() {};
    virtual void AI_In() {};
    virtual void AI_Exit() {};

public:
    CScript(UINT _ScriptType);
    ~CScript();
};

// �ɹ��Լ� ������Ÿ�� ������(CScript �� �ɹ��Լ�, ���ڴ� void, ��ȯŸ�� void)
typedef void(CScript::* SCR_DELEGATE)(void);
typedef void(CScript::* SCR_DELEGATE_1)(DWORD_PTR);
typedef void(CScript::* SCR_DELEGATE_2)(DWORD_PTR, DWORD_PTR);