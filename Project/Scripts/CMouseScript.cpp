#include "pch.h"
#include "CMouseScript.h"

#include <Engine/CDevice.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>


//���콺�� �����̴°ſ� ������ ī�޶� �������� ���콺�� ��ġ�� Ʋ�����°� ������
//�����Ŵ����� ī�޶� ���͸� ��ȯ�ϴ� �Լ��� ����
//�� ī�޶��� ��ġ���� �����Ͱ����� ��ƽ���� ī�޶� �߽���ġ�� ���ϸ� �װ�
//���콺�� ��� ������ �׷��� ī�޶� �����̴°ſ� ���� ���콺�� ������.


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