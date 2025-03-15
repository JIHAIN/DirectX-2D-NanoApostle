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
	// 가속도 구하기 (이전과 동일)
	Vec2 Accel = m_Force / m_Mass;
	m_Velocity += Accel * DT * 100.f;

	// 최대속력 체크 (이전과 동일)
	if (0.f != m_MaxSpeed)
	{
		if (m_MaxSpeed < m_Velocity.Length())
		{
			m_Velocity.Normalize();
			m_Velocity *= m_MaxSpeed;
		}
	}

	// 수정된 마찰력 적용 부분
	if (m_Force == Vec2(0.f, 0.f) && m_Velocity != Vec2(0.f, 0.f))
	{
		// 현재 속도의 크기를 저장
		float currentSpeed = m_Velocity.Length();

		m_thresholdTime += DT;

		// 코사인 기반의 감속 계수 계산
		float frictionCoeff = 0.5f * (1.f + cos(min(m_thresholdTime / m_frictionDuration * XM_PI, XM_PI)));

		// 속도가 특정 임계값 이하면 급격하게 감속
		if (m_thresholdTime >= m_frictionDuration)
		{
			m_Velocity = Vec2(0.f, 0.f);
			m_thresholdTime = 0.f;
		}
		else
		{
			// 현재 속도에 감속 계수를 적용
			m_Velocity *= frictionCoeff;
		}
	}
	else
	{
		// 새로운 힘이 가해지면 타이머 리셋
		m_thresholdTime = 0.f;
	}

	// 나머지 부분은 이전과 동일
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
