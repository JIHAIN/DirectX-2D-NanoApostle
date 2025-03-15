#pragma once
#include <Engine/CScript.h>

class CWallScript :
    public CScript
{
private:
    bool         Player_Move_Lock[4];
    bool         m_Open;

public:
    void SetOpen(bool _OnOff) { m_Open = _OnOff; }
    bool GetOpen() { return m_Open; }

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CWallScript);
    CWallScript();
    ~CWallScript();
};


