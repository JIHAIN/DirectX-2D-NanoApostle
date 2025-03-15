#pragma once
#include "CComponent.h"

class CRigidBody :
    public CComponent
{
private:
    Vec2                m_Force;            // 누적 힘 (크기 + 방향)
    Vec2                m_Velocity;         // 속도    (크기 + 방향)
    float               m_Mass;             // 질량
    float               m_Friction;         // 마찰계수
    float               m_MaxSpeed;         // 최대 속력 제한
    bool                m_IsMove;           // 현재 오브젝트가 움직이는 중인지 아닌지 체크

    // 마찰력 계수를 계산 (0~1 사이의 값)
    float               m_frictionDuration; // 마찰력이 적용되는 시간
    float               m_thresholdTime;    // 마찰력 임계까지의 시간

public:
    void AddForce(Vec2 _Force) { m_Force += _Force; }
    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetMaxSpeed(float _Max) { m_MaxSpeed = _Max; }
    void SetVelocity(Vec2 _Velocity) { m_Velocity = _Velocity; }
    void AddVelociy(Vec2 _Velocity) { m_Velocity += _Velocity; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetfrictDuration(float _Duration) { m_frictionDuration = _Duration; }
    void SetthresholdTime(float _Time) { m_thresholdTime = _Time; }

    Vec2  GetForce() { return m_Force; }
    float GetMass() { return m_Mass; }
    Vec2  GetVelocity() { return m_Velocity; }
    float GetMaxSpeed() { return m_MaxSpeed; }
    bool  IsMove() { return m_IsMove; }

public:
    virtual void FinalTick() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CRigidBody);
    CRigidBody();
    ~CRigidBody();
};

