#include "pch.h"
#include "CPlayer_Attack_Script1.h"

#include "CPlayer_Atk_Obj.h"
#include "CScriptMgr.h"
#include "CPlayer_Ai.h"

CPlayer_Attack_Script1::CPlayer_Attack_Script1()
	: CScript((UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT1)
	, m_AtkDmg(0)
	, m_AtkRange(20)
	, m_AtkTime(0.f)
	, MousDir(0.f, 0.f, 0.f)
{
	SetAI(true);
}

CPlayer_Attack_Script1::~CPlayer_Attack_Script1()
{
}

void CPlayer_Attack_Script1::Begin()
{
	SetAI(true);
}


void CPlayer_Attack_Script1::AI_In()
{
	Vec3 MousePos = g_MouseInfo.MouseWorldPos;
	Vec3 pWPos = Transform()->GetWorldPos();
	Vec3 PlayerMouseDir = MousePos - pWPos;
	MousDir = PlayerMouseDir.Normalize();

	CreateAtk1();
	PlayerAniPlay1(MousDir);
	RigidBody()->AddForce(Vec2(MousDir.x * 300.f, MousDir.y * 300.f));

	m_AtkTime = 0.f;

	// Sound �ε��ϱ�
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\PlayerTentacleSlash.wav", L"Sound\\PlayerTentacleSlash.wav");
	pSound->Play(1, 0.7f, true);
}

void CPlayer_Attack_Script1::AI_Tick()
{
	m_AtkTime += DT;

	if (KEY_TAP(KEY::SPACE))
	{
		P_CHANGE(SCRIPT_TYPE::PLAYER_DASH);
		return;
	}

	if (KEY_TAP(KEY::RBTN))
	{
		P_CHANGE(SCRIPT_TYPE::SHIELD_SCRIPT);
		return;
	}

	// 0.2�ʿ��� 1.2�� ���̿� ���� �Է��� ������ ���� ��������
	if (KEY_TAP(KEY::LBTN) && m_AtkTime >= 0.2f && m_AtkTime <= 1.2f)
	{
		P_CHANGE(SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT2);
		return;
	}

	// �ִϸ��̼��� �����ų� �ð��� �ʰ��Ǹ� �⺻ ���·�
	if (FlipbookPlayer()->isFinish() || m_AtkTime > 1.3f)
	{
		P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
		return;
	}
}


void CPlayer_Attack_Script1::AI_Exit()
{
	m_AtkTime = 0.f;
}

void CPlayer_Attack_Script1::Tick()
{
}

void CPlayer_Attack_Script1::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Attack_Script1::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayer_Attack_Script1::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}



void CPlayer_Attack_Script1::PlayerAniPlay1(Vec3 _Dir)
{
	float angle = atan2(_Dir.x, _Dir.y) * 180.0f / XM_PI;

	// ���� ����
	if (angle >= -45 && angle < 45)// ���� (Up)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Up1, 10.f, false);
	}
	else if (angle >= 45 && angle < 135)// ������ (Right)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right1, 10.f, false);
	}
	else if (angle >= -135 && angle < -45) // ���� (Left)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right1, 10.f, false);
	}
	else // �Ʒ��� (Down)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Down1, 10.f, false);
	}

}

void CPlayer_Attack_Script1::CreateAtk1()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();
	pPos += MousDir * m_AtkRange;

	// atan2�� ����Ͽ� ���� ��� (�޼� ��ǥ��)
	float angle = atan2(MousDir.y, MousDir.x);

	// �⺻ ���� ���� (�� 30�� = ��/6 ����)
	float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

	float baseScaleX = 200.0f;
	float baseScaleY = 200.0f;

	CGameObject* PlayerAtk1 = new CGameObject;
	PlayerAtk1->AddComponent(new CMeshRender);
	PlayerAtk1->AddComponent(new CCollider2D);
	PlayerAtk1->AddComponent(new CFlipbookPlayer);
	PlayerAtk1->AddComponent(new CPlayer_Atk_Obj);
	PlayerAtk1->SetName(L"pAtk1");

	PlayerAtk1->AddComponent(new CLight2D);
	PlayerAtk1->Light2D()->SetLightColor(Vec3(0.1f, 0.1f, 0.1f));
	PlayerAtk1->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	PlayerAtk1->Light2D()->SetRadius(50.f);

	PlayerAtk1->Collider2D()->SetIndependentScale(false);
	PlayerAtk1->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

	PlayerAtk1->Transform()->SetRelativePos(Vec3(pPos.x, pPos.y + 20.f, pPos.z));
	PlayerAtk1->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	PlayerAtk1->Transform()->SetIndependentScale(true);
	PlayerAtk1->Transform()->SetRelativeRotation(Vec3((XM_PI / 3), -(XM_PI / 3), correctedAngle));
	PlayerAtk1->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	PlayerAtk1->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));
	PlayerAtk1->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
	PlayerAtk1->FlipbookPlayer()->Play(0, 160.f, false);

	CreateObject(PlayerAtk1, 4, false);

	

}


void CPlayer_Attack_Script1::SaveComponent(FILE* _File)
{
	fwrite(&m_AtkDmg, sizeof(int), 1, _File);
	fwrite(&m_AtkRange, sizeof(float), 1, _File);
}

void CPlayer_Attack_Script1::LoadComponent(FILE* _File)
{
	fread(&m_AtkDmg, sizeof(int), 1, _File);
	fread(&m_AtkRange, sizeof(float), 1, _File);

	m_AtkTime = 0.f;
	MousDir = Vec3(0.f, 0.f, 0.f);
}


// ùŸ 60 0, ��Ÿ 60, -160
// ùŸ �����Ϸ��� Z���� + 180�� 

// 3Ÿ x�� ��ü ���� 	�̷��� ���ִϱ� x ��� �� 
// float PI = XM_PI / 10;
//Rota.x += PI;
//Rota.y += PI;
//Rota.z += PI;


