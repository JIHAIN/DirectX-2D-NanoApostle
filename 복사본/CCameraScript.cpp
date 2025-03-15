#include "pch.h"
#include "CCameraScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>


CCameraScript::CCameraScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
	, m_CameraSpeed(1500.f)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		CameraOrthgraphicMove();
	else
		CameraPerspectiveMove();
}

void CCameraScript::CameraOrthgraphicMove()
{
	Vec3 vWorldPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::UP))
	{
		vWorldPos.y += DT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vWorldPos.y -= DT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vWorldPos.x -= DT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vWorldPos.x += DT * m_CameraSpeed;
	}

	Transform()->SetRelativePos(vWorldPos);


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
	if (KEY_PRESSED(KEY::LSHIFT))
		Speed *= 5.f;

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