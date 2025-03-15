#pragma once
#include "CComponent.h"

class CRigidBody :
    public CComponent
{
private:
    Vec2                m_Force;            // ���� �� (ũ�� + ����)
    Vec2                m_Velocity;         // �ӵ�    (ũ�� + ����)
    float               m_Mass;             // ����
    float               m_Friction;         // �������
    float               m_MaxSpeed;         // �ִ� �ӷ� ����
    bool                m_IsMove;           // ���� ������Ʈ�� �����̴� ������ �ƴ��� üũ

    // ������ ����� ��� (0~1 ������ ��)
    float               m_frictionDuration; // �������� ����Ǵ� �ð�
    float               m_thresholdTime;    // ������ �Ӱ������ �ð�

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

