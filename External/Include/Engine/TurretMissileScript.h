#pragma once
#include "CScript.h"

class TurretMissileScript :
    public CScript
{
private:
    Vec3        m_Velocity;
    float       m_DeleteTime;
    float       m_ScaleTime;
    bool        m_smaller;

    Vec3        m_Originalsize;
    Vec3        m_TurretPos;

    CGameObject* m_TargetPlayer;

public:
    void SetTurretPos(Vec3 _Pos) { m_TurretPos = _Pos; }
    Vec3 GetVelocity() { return m_Velocity; }

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);


public:
    CLONE(TurretMissileScript);
    TurretMissileScript();
    ~TurretMissileScript();
};

