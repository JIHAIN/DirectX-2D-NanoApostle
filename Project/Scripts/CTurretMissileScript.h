#pragma once
#include <Engine/CScript.h>

class CTurretMissileScript :
    public CScript
{
private:
    Vec3        m_Velocity;
    float       m_DeleteTime;
    float       m_ScaleTime;
    bool        m_smaller;
    float       m_Speed;

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
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CTurretMissileScript);
    CTurretMissileScript();
    ~CTurretMissileScript();
};

