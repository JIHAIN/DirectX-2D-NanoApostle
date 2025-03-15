#include "pch.h"
#include "CEB.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayer_Ai.h"
#include "CPlayerScript.h"

CEB::CEB()
	: CScript((UINT)SCRIPT_TYPE::EB)
	, m_Goal(false)
	, m_Player(nullptr)
{
}

CEB::~CEB()
{
}

void CEB::Begin()
{
	m_Goal = false;
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Player = CurLevel->FindObjectByName(L"Player");
}

void CEB::Tick()
{
	if (!m_Goal)
	{
		//-371~ -243 로 가야함
		Vec3 myPos = GetOwner()->Transform()->GetRelativePos();

		if (myPos.y < -243.f)
		{
			myPos.y += 50 * DT;
			myPos.z = 101.f;
			m_Player->Transform()->SetRelativePos(myPos);
			myPos.z = 102.f;
			Transform()->SetRelativePos(myPos);
		}
		else if (myPos.y >= -243.f)
		{
			myPos.y = -243.f;
			Transform()->SetRelativePos(myPos);
			myPos.z = 10.f;
			m_Player->Transform()->SetRelativePos(myPos);

			m_Goal = true;
		}

	}

}

void CEB::SaveComponent(FILE* _File)
{
}

void CEB::LoadComponent(FILE* _File)
{
}


