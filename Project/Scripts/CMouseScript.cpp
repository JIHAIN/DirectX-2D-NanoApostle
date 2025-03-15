#include "pch.h"
#include "CMouseScript.h"

#include <Engine/CDevice.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>


//마우스가 움직이는거와 별도로 카메라가 움직여서 마우스의 위치가 틀어지는거 때문에
//랜더매니저에 카메라 벡터를 반환하는 함수를 만들어서
//그 카메라의 위치값을 가져와가지고 매틱마다 카메라 중심위치가 변하면 그걸
//마우스의 포즈에 더해줌 그래서 카메라가 움직이는거와 같이 마우스가 움직임.


CMouseScript::CMouseScript()
	: CScript((UINT)SCRIPT_TYPE::MOUSESCRIPT)
{
}

CMouseScript::~CMouseScript()
{
}

void CMouseScript::Begin()
{
}

void CMouseScript::Tick()
{
	if (GetFocus())
	{
		vector<CCamera*> Cam = CRenderMgr::GetInst()->GetVecCamer();

		if (Cam.size() != 0)
		{
			/*for (int i = 0; i < Cam.size(); ++i)
			{
				if (Cam[i] != nullptr)
				{
					CCamera* Cam0 = Cam[1];

					Vec3 CamPos = Cam0->Transform()->GetWorldPos();

					Vec2 vMousePos = g_MouseInfo.MousePos;

					Transform()->SetRelativePos(Vec3(vMousePos.x + CamPos.x, vMousePos.y + CamPos.y, 1.f));
					break;
				}
			}*/
			CCamera* Cam0 = Cam[1];

			Vec3 CamPos = Cam0->Transform()->GetWorldPos();

			Vec2 vMousePos = g_MouseInfo.MousePos;

			Transform()->SetRelativePos(Vec3(vMousePos.x + CamPos.x, vMousePos.y + CamPos.y, 1.f));
		}
	}

	g_MouseInfo.MouseWorldPos = Transform()->GetWorldPos();
}

void CMouseScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMouseScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMouseScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}