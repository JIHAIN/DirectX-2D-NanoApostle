#pragma once
#include <Engine/CScript.h>

class CGroundBoundary :
    public CScript
{
private:
    CGameObject* m_Player;

    Vec2 PrevPos;
    Vec2 vOthObjPos;

    bool m_CanExit[4];  // 0: 왼쪽, 1: 오른쪽, 2: 위, 3: 아래 

public:
    void SetExitDirection(int _dir, bool _canExit) { m_CanExit[_dir] = _canExit; }

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CGroundBoundary);
    CGroundBoundary();
    ~CGroundBoundary();
};


