#include "pch.h"
#include "CWallScript.h"

#include "CPlayer_Ai.h"
#include "CPlayerScript.h"

CWallScript::CWallScript()
	: CScript((UINT)SCRIPT_TYPE::WALLSCRIPT)
	, Player_Move_Lock{true, true, true, true}
	, m_Open(false)
{
}

CWallScript::~CWallScript()
{
}

void CWallScript::Begin()
{
	m_Open = false;
	memset(Player_Move_Lock, true, sizeof(Player_Move_Lock));
}

void CWallScript::Tick()
{

}

void CWallScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() != L"Player" || m_Open)
		return;

	CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

	// ���� �÷��̾��� �߽���
	Vec2 wallCenter = Vec2(_Collider->GetColliderWorldMat().Translation().x,
		_Collider->GetColliderWorldMat().Translation().y);
	Vec2 playerCenter = Vec2(_OtherCollider->GetColliderWorldMat().Translation().x,
		_OtherCollider->GetColliderWorldMat().Translation().y);

	// z�� ȸ�� Ȯ�� (90���� �������)
	float rotationZ = Transform()->GetRelativeRotation().z;
	bool isRotated90 = abs(abs(rotationZ) - XM_PI / 2) < 0.1f;  // 90�� ȸ�� ���� üũ

	// �浹ü�� ũ��
	Vec2 wallScale;
	if (isRotated90)
	{
		// 90�� ȸ���� ��� x,y ����
		wallScale = Vec2(_Collider->Transform()->GetWorldScale().y,
			_Collider->Transform()->GetWorldScale().x);
	}
	else
	{
		// �Ϲ����� ���
		wallScale = Vec2(_Collider->Transform()->GetWorldScale().x,
			_Collider->Transform()->GetWorldScale().y);
	}

	Vec2 playerScale = Vec2(_OtherCollider->Transform()->GetWorldScale().x,
		_OtherCollider->Transform()->GetWorldScale().y);

	// �Ÿ� ���
	Vec2 distance = playerCenter - wallCenter;

	// �� ������ ��ħ
	float overlapX = (wallScale.x + playerScale.x) * 0.5f - abs(distance.x);
	float overlapY = (wallScale.y + playerScale.y) * 0.5f - abs(distance.y);

	if (overlapX < overlapY)
	{
		// X�� ���� �浹
		if (distance.x > 0)
		{
			// �÷��̾ �����ʿ� ����
			playerScript->Set_Player_Move_limit(0, false);
			Player_Move_Lock[0] = false;
		}
		else
		{
			// �÷��̾ ���ʿ� ����
			playerScript->Set_Player_Move_limit(1, false);
			Player_Move_Lock[1] = false;
		}
	}
	else
	{
		// Y�� ���� �浹
		if (distance.y > 0)
		{
			// �÷��̾ ���� ����
			playerScript->Set_Player_Move_limit(3, false);
			Player_Move_Lock[3] = false;
		}
		else
		{
			// �÷��̾ �Ʒ��� ����
			playerScript->Set_Player_Move_limit(2, false);
			Player_Move_Lock[2] = false;
		}
	}
}

void CWallScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT);

		for (int i = 0; i < 4; ++i)
		{
			if (Player_Move_Lock[i] == false)
			{
				playerScript->Set_Player_Move_limit(i, true);
			}
		}

		// �� �ᱺ�Ÿ� Ǯ���� 

		memset(Player_Move_Lock, true, sizeof(Player_Move_Lock));// �ٽ� Ʈ��� ����

		if (m_Open)
		{
			m_Open = false;
		}
	}

}

void CWallScript::SaveComponent(FILE* _File)
{
}

void CWallScript::LoadComponent(FILE* _File)
{
}

