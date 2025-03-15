#include "pch.h"
#include "CCameraScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CCameraScript::CCameraScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
	, m_CameraSpeed(1500.f)
	, m_Player(nullptr)
	, m_CurrentTarget(nullptr)
	, m_NextTarget(nullptr)
	, m_TargetDuration(0.f)
	, m_TransitionSpeed(0.f)
	, m_CurrentTime(0.f)
	, m_TransitionProgress(0.f)
	, m_IsTransitioning(false) {
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::SetTemporaryTarget(CGameObject* _target, float _duration, float _transitionSpeed)
{
	if (_target == nullptr)
		return;

	m_NextTarget = _target;
	m_TargetDuration = _duration;
	m_TransitionSpeed = _transitionSpeed;
	m_CurrentTime = 0.f;
	m_TransitionProgress = 0.f;
	m_IsTransitioning = true;
}

void CCameraScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		CameraOrthgraphicMove();
	else
		CameraPerspectiveMove();
}

void CCameraScript::Begin()
{
	m_CameraSpeed = 1500.f;
	m_CurrentTarget = nullptr;
	m_NextTarget = nullptr;
	m_TargetDuration = 0.f;
	m_TransitionSpeed = 0.f;
	m_CurrentTime = 0.f;
	m_TransitionProgress = 0.f;
	m_IsTransitioning = false;

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Player = CurLevel->FindObjectByName(L"Player");
}

void CCameraScript::CameraOrthgraphicMove()
{
	if (m_IsTransitioning)
	{
		// 타겟 전환 중
		m_TransitionProgress += m_TransitionSpeed * DT;

		if (m_TransitionProgress >= 1.f)
		{
			// 전환 완료
			m_TransitionProgress = 1.f;
			m_CurrentTarget = m_NextTarget;
			m_IsTransitioning = false;
		}

		// 현재 위치와 목표 위치 사이를 보간
		Vec3 currentPos = m_CurrentTarget ? m_CurrentTarget->Transform()->GetRelativePos() : m_Player->Transform()->GetRelativePos();
		Vec3 targetPos = m_NextTarget->Transform()->GetRelativePos();
		Vec3 lerpedPos = Lerp(currentPos, targetPos, m_TransitionProgress);
		Transform()->SetRelativePos(Vec3(lerpedPos.x, lerpedPos.y, 0.f));
	}
	else if (m_CurrentTarget)  // 전환이 끝난 후 타겟을 바라보는 중
	{
		m_CurrentTime += DT;
		if (m_CurrentTime >= m_TargetDuration)
		{
			// 플레이어로 돌아가기 시작
			SetTemporaryTarget(m_Player, 0.f, m_TransitionSpeed);
		}
		else
		{
			// 타겟 위치로 이동
			Transform()->SetRelativePos(m_CurrentTarget->Transform()->GetRelativePos());
		}
	}
	else if (m_Player)  // 일반 상태 - 플레이어 추적
	{
		Vec3 targetPos = m_Player->Transform()->GetRelativePos();
		Vec3 currentPos = Transform()->GetRelativePos();

		float dist = (targetPos - currentPos).Length();
		float followSpeed = min(1.0f, 1.0f * DT);

		if (dist > 100.f)
			followSpeed = 1.0f;
		else if (dist > 10.f)
			followSpeed = 0.5f;

		Vec3 newPos = currentPos + (targetPos - currentPos) * followSpeed;
		Transform()->SetRelativePos(Vec3(newPos.x, newPos.y, 0.f));
	}


	float Scale = Camera()->GetScale();

	if (KEY_PRESSED(KEY::NUM_0))
	{
		Scale += DT;
	}

	if (KEY_PRESSED(KEY::NUM_1))
	{
		Scale -= DT;
		if (Scale < 0.1f)
			Scale = 0.1f;
	}

	Camera()->SetScale(Scale);
}

void CCameraScript::CameraPerspectiveMove()
{
	float Speed = m_CameraSpeed;

	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vRight= Transform()->GetLocalDir(DIR_TYPE::RIGHT);	

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += vFront * DT * Speed;
	if(KEY_PRESSED(KEY::S))
		vPos -= vFront * DT * Speed;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * DT * Speed;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * DT * Speed;

	Transform()->SetRelativePos(vPos);

	// 마우스 방향에 따른 오브젝트 회전
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();

		Vec2 vDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += vDir.x * DT * 15.f;
		vRot.x += vDir.y * DT * 10.f;

		Transform()->SetRelativeRotation(vRot);
	}
}


void CCameraScript::SaveComponent(FILE* _File)
{
	fwrite(&m_CameraSpeed, sizeof(float), 1, _File);
}

void CCameraScript::LoadComponent(FILE* _File)
{
	fread(&m_CameraSpeed, sizeof(float), 1, _File);
}