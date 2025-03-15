#include "pch.h"
#include "CBoss_Atk_Obj.h"
#include <Engine/CSprite.h>
#include <Engine/CFlipbook.h>
#include "CBoss_Ai.h"

CBoss_Atk_Obj::CBoss_Atk_Obj()
	: CScript((UINT)SCRIPT_TYPE::BOSS_ATK_OBJ)
	, m_firstEnd(false)
	, m_Name()
	, m_Time(0.f)
{
}

CBoss_Atk_Obj::~CBoss_Atk_Obj()
{
}


void CBoss_Atk_Obj::Begin()
{
	m_firstEnd = false;
	m_Name = GetOwner()->GetName();
	m_Time = 0.f;
}

void CBoss_Atk_Obj::Tick()
{
	m_Time += DT;// *3.f;
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, Vec4(m_Time, 0.f, 0.f, m_Time));

	if (m_Name == L"B_Atk3")
	{
		if (GetOwner()->FlipbookPlayer()->isFinish())
		{
			DestroyObject(GetOwner());
		}
	}
	else
	{
		if (!m_firstEnd) // 첫번째 애니메이션 일때
		{
			//GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, Vec4(0.8f, 0.f, 0.f, 0.f));

			if (GetOwner()->FlipbookPlayer()->isFinish()) // 첫번째가 끝나면 각도 좀 틀어서 2번째 재생해줌
			{
				Vec3 Pos = Transform()->GetRelativePos();
				Vec3 Rota = Transform()->GetRelativeRotation();
				bool left = CBoss_Ai::IsLeft();

				m_firstEnd = true;

				if (m_Name == L"B_Atk1")
				{
					Vec3 Scale = Transform()->GetRelativeScale();

					Scale.x *= 1.3f;
					Pos.x *= 1.2f;
					Pos.y -= 20.f;
					Rota.z += (XM_PI / 18);

					Transform()->SetRelativeScale(Scale);
					Transform()->SetRelativePos(Pos);
					Transform()->SetRelativeRotation(Rota);

				}
				else if (m_Name == L"B_Atk2")
				{
					//Pos.x *= 1.2f;
					//Pos.y -= 20.f;
					// s 400 600 r 180 180 - 110
					Rota.x = 0;
					Rota.y = XM_PI;

					if (left)
					{
						Rota.z = -((XM_PI / 18.f) * 11.f);
					}
					else
					{
						Rota.z = ((XM_PI / 18.f) * 11.f);
					}

					Transform()->SetRelativePos(Pos);
					Transform()->SetRelativeRotation(Rota);

				}

				FlipbookPlayer()->Play(1, 60.f, false);
			}
		}
		else // 두번째 애니메이션 일때
		{
			//GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, Vec4(0.5f, 0.f, 0.f, 0.f));

			Vec3 Rota = Transform()->GetRelativeRotation();
			bool left = CBoss_Ai::IsLeft();

			if (m_Name == L"B_Atk1")
			{
				Rota.z -= XM_PI / 3.33f * DT;
			}
			else if (m_Name == L"B_Atk2")
			{
				if (left)
				{
					Rota.z -= XM_PI / 3.33f * DT;
				}
				else
				{
					Rota.z += XM_PI / 3.33f * DT;
				}
			}
			Transform()->SetRelativeRotation(Rota);

			if (GetOwner()->FlipbookPlayer()->isFinish()) // 두번째 애니메이션 이라면 끝나고 삭제
			{
				DestroyObject(GetOwner());
			}
		}
	}

}

void CBoss_Atk_Obj::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Atk_Obj::SaveComponent(FILE* _File)
{
}

void CBoss_Atk_Obj::LoadComponent(FILE* _File)
{
}

