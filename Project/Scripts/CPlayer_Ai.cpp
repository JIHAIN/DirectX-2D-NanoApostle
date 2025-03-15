#include "pch.h"
#include "CPlayer_Ai.h"

#include <Engine/CLayer.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include "CUI_Hpbar.h"
#include "CShield_Script.h"

CPlayer_Ai::CPlayer_Ai()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_AI)
	, m_CurScript(nullptr)
	, m_HitOn(false)
	, HitEffect(nullptr)
	, m_Invincible(false)
	, m_PlayerPrevPos(Vec3(0.f, 0.f, 0.f))
	, m_GroggyMonster(nullptr)
	, m_state(CPlayer_Dash_State::Down)
	, m_InvinTime(0.f)
{
	
}

CPlayer_Ai::~CPlayer_Ai()
{
	if (IS_SLOW)
		SLOW;
}

void CPlayer_Ai::Begin()
{
	m_CurScript = nullptr;
	m_HitOn = false;
	HitEffect = nullptr;
	m_Invincible = false;
	m_PlayerPrevPos = Vec3(0.f, 0.f, 0.f);
	m_GroggyMonster = nullptr;
	m_state = CPlayer_Dash_State::Down;
	m_InvinTime = 0.f;

	wstring LevelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();

	if (LevelName == L"Boss")
	{
		m_CurScript = GetOwner()->GetScript((UINT)SCRIPT_TYPE::PLAYER_BOSSROOM);
		m_CurScript->AI_In();
	}
	else
	{
		m_CurScript = GetOwner()->GetScript((UINT)SCRIPT_TYPE::PLAYER_MAIN);
		m_CurScript->AI_In();
	}

}




void CPlayer_Ai::Tick()
{
	if (m_Invincible) // ������ 1��
	{
		m_InvinTime += DT;

		if (m_InvinTime >= 1.f)
		{
			m_Invincible = false;
			m_InvinTime = 0.f;
		}
	}

	if (m_HitOn)
	{
		static float HitTime = 0.f;
		if (!IS_SLOW)
			SLOW;
		
		HitTime += g_Data.DT_Engine;
		HitEffect->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_3, HitTime);
		if (HitTime >= 0.5f) // 0.5�ʶ��� �ӵ� ����, ����Ʈ ���ְ�
		{
			SLOW;
			m_HitOn = false;
			HitTime = 0.f;
			DestroyObject(HitEffect);
		}
	}

	PlayerDir();
	PlayerLeft();

	if (nullptr != m_CurScript)
		m_CurScript->AI_Tick();
	
	m_PlayerPrevPos = Transform()->GetRelativePos();
}

void CPlayer_Ai::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 6)
	{
		CShield_Script* OtherShield = (CShield_Script*)GetOwner()->GetScript(SCRIPT_TYPE::SHIELD_SCRIPT);
		bool Parry = false;

		if (OtherShield != nullptr)
		{
			Parry = OtherShield->IsParry();
		}

		// �¾����� ���ڰŸ��鼭 �������� ȿ��
		if (!m_Invincible && !Parry) // ���� ���¶� �и� ���� �ƴҶ���
		{
			// Sound �ε��ϱ�
			Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\PlayerDamage(ElerctSSP05WEE+ElerctSCV04+ElerctSAMI03).wav", L"Sound\\PlayerDamage(ElerctSSP05WEE+ElerctSCV04+ElerctSAMI03).wav");
			pSound->Play(1, 0.5f, false);

			HitEffect = new CGameObject;
			HitEffect->SetName(L"Hit");
			HitEffect->AddComponent(new CMeshRender);

			HitEffect->Transform()->SetRelativePos(Transform()->GetRelativePos());
			HitEffect->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			HitEffect->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HitEffectMtrl"));
			HitEffect->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
			CreateObject(HitEffect, 4, false);

			m_Invincible = true; // �������� ���ְ�
			m_HitOn = true;		 // ��Ʈ ����Ʈ ����

			CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* HPObj = CurLevel->FindObjectByName(L"HP");
			CUI_Hpbar* Hpbar = (CUI_Hpbar*)HPObj->GetScript(SCRIPT_TYPE::UI_HPBAR);
			Hpbar->HitHP(30.f);
		}
	}


}

void CPlayer_Ai::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}
void CPlayer_Ai::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}


void CPlayer_Ai::SaveComponent(FILE* _File)
{
}

void CPlayer_Ai::LoadComponent(FILE* _File)
{
}

void CPlayer_Ai::PlayerLeft() // �÷��̾� ���� �����ϱ�
{
	Vec3 PlayerScale = Transform()->GetRelativeScale();

	if (PlayerScale.x > 0.f && (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) &&
		!(KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S) ||
		KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D) ||
		KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W)))
	{
		PlayerScale.x *= -1.f;  // ������ ���� ���� �� AŰ ������ ��������
	}
	else if (PlayerScale.x < 0.f &&
			(KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S) ||
			KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D) ||
			KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W)))
	{
		PlayerScale.x *= -1.f;  // ���� ���� ���� �� S,D,WŰ ������ ����������
	}

	if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::RBTN))
	{
		Vec3 MousePos = g_MouseInfo.MouseWorldPos;
		Vec3 pWPos = Transform()->GetWorldPos();
		Vec3 PlayerMouseDir = MousePos - pWPos;
		Vec3 MouseDir = PlayerMouseDir.Normalize();

		// ���콺 ���� ��� (-180 ~ 180��)
		float angle = atan2(MouseDir.x, MouseDir.y) * 180.0f / XM_PI;

		// �� ���⺰ ó��
		if (angle >= -45.f && angle < 45.f)  // ��
		{
			if (PlayerScale.x < 0.f)
				PlayerScale.x *= -1.f;
		}
		else if (angle >= 45.f && angle < 135.f)  // ������
		{
			if (PlayerScale.x < 0.f)
				PlayerScale.x *= -1.f;
		}
		else if (angle >= -135.f && angle < -45.f)  // ����
		{
			if (PlayerScale.x > 0.f)
				PlayerScale.x *= -1.f;
		}
		else  // �Ʒ� (-180 ~ -135 �Ǵ� 135 ~ 180)
		{
			if (PlayerScale.x < 0.f)
				PlayerScale.x *= -1.f;
		}
	}

	Transform()->SetRelativeScale(PlayerScale);
}

void CPlayer_Ai::PlayerDir()
{
	// �밢�� ���� ó��
	if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::W))
	{
		m_PlayerDir = Vec3(-1.f, 1.f, 0.f);
		m_state = CPlayer_Dash_State::LeftUP;
	}
	else if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::S))
	{
		m_PlayerDir = Vec3(-1.f, -1.f, 0.f);
		m_state = CPlayer_Dash_State::LeftDOWN;
	}
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::W))
	{
		m_PlayerDir = Vec3(1.f, 1.f, 0.f);
		m_state = CPlayer_Dash_State::RightUP;
	}
	else if (KEY_PRESSED(KEY::D) && KEY_PRESSED(KEY::S))
	{
		m_PlayerDir = Vec3(1.f, -1.f, 0.f);
		m_state = CPlayer_Dash_State::RightDOWN;
	}
	// ���� ���� ó��
	else if (KEY_PRESSED(KEY::A))
	{
		m_PlayerDir = Vec3(-1.f, 0.f, 0.f);
		m_state = CPlayer_Dash_State::Left;
	}
	else if (KEY_PRESSED(KEY::D))
	{
		m_PlayerDir = Vec3(1.f, 0.f, 0.f);
		m_state = CPlayer_Dash_State::Right;
	}
	else if (KEY_PRESSED(KEY::W))
	{
		m_PlayerDir = Vec3(0.f, 1.f, 0.f);
		m_state = CPlayer_Dash_State::Up;
	}
	else if (KEY_PRESSED(KEY::S))
	{
		m_PlayerDir = Vec3(0.f, -1.f, 0.f);
		m_state = CPlayer_Dash_State::Down;
	}

	// �Էµ� ���� ���� ����ȭ
	if (m_PlayerDir.x != 0.f || m_PlayerDir.y != 0.f)
	{
		float length = sqrt(m_PlayerDir.x * m_PlayerDir.x + m_PlayerDir.y * m_PlayerDir.y);
		m_PlayerDir.x /= length;
		m_PlayerDir.y /= length;
	}
}

bool CPlayer_Ai::IsDeathState()
{
	if(	m_CurScript == GetOwner()->GetScript(SCRIPT_TYPE::PLAYER_DEATH))
		return true;
	else
		return false;
}

void CPlayer_Ai::Set_Groggy_Monster(CGameObject* _Monster)
{
	m_GroggyMonster = _Monster;

	ChangeAi(SCRIPT_TYPE::PLAYER_HOOK_SCRIPT);
}

void CPlayer_Ai::ChangeAi(SCRIPT_TYPE _Type)
{
	CScript* pNextAi = GetOwner()->GetScript(_Type);

	// ���� ���� ������ Exit �Լ� ����
	if (nullptr != m_CurScript)
	{
		m_CurScript->AI_Exit();
	}

	//���ο� ���·�
	m_CurScript = pNextAi;
	assert(m_CurScript);

	// �������ֱ�
	m_CurScript->AI_In();
}

void CPlayer_Ai::Ai_PlayerAni(Player_Ani_State _Type, float _Time, bool _Re)
{
	FlipbookPlayer()->Play((int)_Type, _Time, _Re);
}


