#include "pch.h"
#include "CRigidBody.h"

#include "components.h"
#include "CEngine.h"
#include "CTimeMgr.h"

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Force(0.f, 0.f)
	, m_Velocity(0.f, 0.f)
	, m_MaxSpeed(3000.f)
	, m_frictionDuration(0.5f)
	, m_thresholdTime(0.5f)
	, m_IsMove(false)
	, m_Friction(0.f)
	, m_Mass(1.f)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::FinalTick()
{
	// ���ӵ� ���ϱ� (������ ����)
	Vec2 Accel = m_Force / m_Mass;
	m_Velocity += Accel * DT * 100.f;

	// �ִ�ӷ� üũ (������ ����)
	if (0.f != m_MaxSpeed)
	{
		if (m_MaxSpeed < m_Velocity.Length())
		{
			m_Velocity.Normalize();
			m_Velocity *= m_MaxSpeed;
		}
	}

	// ������ ������ ���� �κ�
	if (m_Force == Vec2(0.f, 0.f) && m_Velocity != Vec2(0.f, 0.f))
	{
		// ���� �ӵ��� ũ�⸦ ����
		float currentSpeed = m_Velocity.Length();

		m_thresholdTime += DT;

		// �ڻ��� ����� ���� ��� ���
		float frictionCoeff = 0.5f * (1.f + cos(min(m_thresholdTime / m_frictionDuration * XM_PI, XM_PI)));

		// �ӵ��� Ư�� �Ӱ谪 ���ϸ� �ް��ϰ� ����
		if (m_thresholdTime >= m_frictionDuration)
		{
			m_Velocity = Vec2(0.f, 0.f);
			m_thresholdTime = 0.f;
		}
		else
		{
			// ���� �ӵ��� ���� ����� ����
			m_Velocity *= frictionCoeff;
		}
	}
	else
	{
		// ���ο� ���� �������� Ÿ�̸� ����
		m_thresholdTime = 0.f;
	}

	// ������ �κ��� ������ ����
	Vec2 vFinalVelocity = m_Velocity;
	Vec3 vObjPos = GetOwner()->Transform()->GetRelativePos();
	GetOwner()->Transform()->SetRelativePos(vObjPos + vFinalVelocity * DT);

	if (vObjPos == GetOwner()->Transform()->GetRelativePos())
		m_IsMove = false;
	else
		m_IsMove = true;

	m_Force = Vec2(0.f, 0.f);
}

void CRigidBody::SaveComponent(FILE* _File)
{

	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
	fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_frictionDuration, sizeof(float), 1, _File);
	fwrite(&m_thresholdTime, sizeof(float), 1, _File);
}

void CRigidBody::LoadComponent(FILE* _File)
{
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
	fread(&m_MaxSpeed, sizeof(float), 1, _File);
	fread(&m_frictionDuration, sizeof(float), 1, _File);
	fread(&m_thresholdTime, sizeof(float), 1, _File);
}
