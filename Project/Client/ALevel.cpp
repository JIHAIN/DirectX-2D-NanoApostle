#include "pch.h"
#include "ALevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>
#include "Scripts.h"


void ALevel::CreateALevel()
{

	CLevel* m_CurLevel = new CLevel;

	// �׽�Ʈ ������ ���� ������ ����
	ChangeLevel(m_CurLevel, LEVEL_STATE::STOP);

	m_CurLevel->GetLayer(0)->SetName(L"Background");
	m_CurLevel->GetLayer(1)->SetName(L"Tile");
	m_CurLevel->GetLayer(2)->SetName(L"Default");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"PlayerObject");
	m_CurLevel->GetLayer(5)->SetName(L"Monster");
	m_CurLevel->GetLayer(6)->SetName(L"MonsterObject");

	CGameObject* pObject = nullptr;

	// ==========
	// MainCamera
	// ==========
	pObject = new CGameObject;
	pObject->SetName(L"MainCamera");
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCameraScript);

	// MainCamera �� ����
	pObject->Camera()->SetPriority(0);

	// ��� ���̾ �ﵵ�� ����
	pObject->Camera()->LayerCheckAll();

	// ī�޶� ������� ����
	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	m_CurLevel->AddObject(0, pObject, false);

	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(3, 5); // �÷��̾�� ����
	CCollisionMgr::GetInst()->CollisionCheck(3, 6); // �÷��̾�� ���� ������Ʈ


	// ���� ���� ����
	::ChangeLevelState(LEVEL_STATE::PLAY);
}
