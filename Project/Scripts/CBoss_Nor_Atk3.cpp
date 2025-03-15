#include "pch.h"
#include "CBoss_Nor_Atk3.h"
#include "CBoss_Ai.h"
#include "CBoss_Atk_Obj.h"

CBoss_Nor_Atk3::CBoss_Nor_Atk3()
	: CScript((UINT)SCRIPT_TYPE::BOSS_NOR_ATK3)
	, pParticle(nullptr)
	, pParticle1(nullptr)

{
	SetAI(true);

}

CBoss_Nor_Atk3::~CBoss_Nor_Atk3()
{
}


void CBoss_Nor_Atk3::Begin()
{
	SetAI(true);
	m_Atk = false;
	pParticle = nullptr;
	pParticle1 = nullptr;
}

void CBoss_Nor_Atk3::AI_In()
{
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Nor_Atk3, 12.f, false);
	m_Atk = false;

	FlipbookPlayer()->SetTerm(1, 0.1f);


	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	Vec3 pos1 = pos;

	bool left = CBoss_Ai::IsLeft();
	if (left)
	{
		pos.x += 166.f;
		pos1.x += 180.f;
		pos1.y -= 140.f;
	}
	else
	{
		pos.x -= 166.f;
		pos1.x -= 180.f;
		pos1.y -= 140.f;
	}

	//FlipbookPlayer()->Pause();

	pParticle = new CGameObject;
	pParticle->SetName(L"Atk3_Particle");
	pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare8_1024.png")));
	pParticle->Transform()->SetRelativePos(pos);
	CreateObject(pParticle, 2, false);

	pParticle1 = new CGameObject;
	pParticle1->SetName(L"Atk3_Particle");
	pParticle1->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare8_1024.png")));
	pParticle1->Transform()->SetRelativePos(pos1);
	CreateObject(pParticle1, 2, false);

	// Sound 로딩하기
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\Boss_Berserker_TripleSlash_3.wav", L"Sound\\Boss_Berserker_TripleSlash_3.wav");
	pSound->Play(1, 0.5f, true);
}


void CBoss_Nor_Atk3::AI_Tick()
{
	if (FlipbookPlayer()->GetCurIdx() == 7 && !m_Atk)
	{
		CreateAtk();
		m_Atk = true;
	}

	if (FlipbookPlayer()->isFinish())
	{
		int randomValue = rand() % 2;

		if (randomValue == 0)
		{
			B_CHANGE(SCRIPT_TYPE::BOSS_IDLE);
		}
		else
		{
			B_CHANGE(SCRIPT_TYPE::BOSS_BOTTOM_ATK);
		}

		//B_CHANGE(SCRIPT_TYPE::BOSS_JUMP_ATK);
		//B_CHANGE(SCRIPT_TYPE::BOSS_NOR_ATK1);

		return;
	}
}

void CBoss_Nor_Atk3::AI_Exit()
{
	DestroyObject(pParticle);
	DestroyObject(pParticle1);

}


void CBoss_Nor_Atk3::SaveComponent(FILE* _File)
{
}

void CBoss_Nor_Atk3::LoadComponent(FILE* _File)
{
}

void CBoss_Nor_Atk3::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Nor_Atk3::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Nor_Atk3::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


void CBoss_Nor_Atk3::CreateAtk()
{
	float baseScaleX = 0.0f;
	float baseScaleY = 0.0f;
	Vec3 Rota = Vec3(0.f, 0.f, 0.f);

	Vec3 Pos = GetOwner()->Transform()->GetRelativePos();

	bool left = CBoss_Ai::IsLeft(); // 보스가 왼쪽 보는지 오른쪽 보는지
	float Angle = XM_PI / 18; // 10도

	CGameObject* BossAtk = new CGameObject;
	BossAtk->AddComponent(new CMeshRender);
	BossAtk->AddComponent(new CCollider2D);
	BossAtk->AddComponent(new CFlipbookPlayer);
	BossAtk->AddComponent(new CBoss_Atk_Obj);
	BossAtk->SetName(L"B_Atk3");

	BossAtk->Collider2D()->SetIndependentScale(false);
	BossAtk->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

	BossAtk->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	BossAtk->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossSkillMtrl"));
	//BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj_First"));
	BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj"));
	BossAtk->Transform()->SetIndependentScale(true);

	if (left) // 보스가 왼쪽을 보고있다면
	{
		// 왼쪽
		Pos.x -= 40.f;
		Pos.y -= 20.f;
		baseScaleX = -600.0f;
		baseScaleY = 700.0f;
		Rota = Vec3(Angle * -5.f, Angle * -4.f, 0.f);
	}
	else // 오른쪽
	{
		Pos.x += 40.f;
		Pos.y -= 20.f;
		baseScaleX = 600.0f;
		baseScaleY = 700.0f;

		Rota = Vec3(Angle * -5.f, Angle * 4.f, 0.f);
	}
	BossAtk->Transform()->SetRelativePos(Pos);
	BossAtk->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	BossAtk->Transform()->SetRelativeRotation(Rota);
	BossAtk->FlipbookPlayer()->Play(0, 80.f, false);
	CreateObject(BossAtk, 6, false);

	BossAtk = nullptr;

	BossAtk = new CGameObject;
	BossAtk->AddComponent(new CMeshRender);
	BossAtk->AddComponent(new CCollider2D);
	BossAtk->AddComponent(new CFlipbookPlayer);
	BossAtk->AddComponent(new CBoss_Atk_Obj);
	BossAtk->SetName(L"B_Atk3");

	BossAtk->Collider2D()->SetIndependentScale(false);
	BossAtk->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

	BossAtk->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	BossAtk->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossSkillMtrl"));
	//BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj_First"));
	BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj"));
	BossAtk->Transform()->SetIndependentScale(true);


	if (left) // 보스가 왼쪽을 보고있다면
	{
		Pos = GetOwner()->Transform()->GetRelativePos();
		Pos.x -= 30.f;
		Pos.y -= 11.f;
		baseScaleX = 650.0f;
		baseScaleY = 600.0f;
		Rota = Vec3(0.f, XM_PI, Angle);
	}
	else
	{
		Pos = GetOwner()->Transform()->GetRelativePos();
		Pos.x -= 30.f;
		Pos.y -= 11.f;
		baseScaleX = 650.0f;
		baseScaleY = 600.0f;
		Rota = Vec3(0.f, 0.f, Angle);
	}

	Pos.z -= 1.f;
	BossAtk->Transform()->SetRelativePos(Pos);
	BossAtk->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	BossAtk->Transform()->SetRelativeRotation(Rota);
	BossAtk->FlipbookPlayer()->Play(0, 80.f, false);

	CreateObject(BossAtk, 6, false);

}

tParticleModule CBoss_Nor_Atk3::ParticleSetting()
{
	// Flare8_1024.png
	tParticleModule m_Module = {};

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_Module.SpawnRate = 0;
	m_Module.vSpawnColor = Vec4(1.f, 0.6f, 0.f, 1.f);
	m_Module.MinLife = 0.f;
	m_Module.MaxLife = 0.6f;
	m_Module.vSpawnMinScale = Vec3(30.f, 30.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(30.f, 30.f, 1.f);

	m_Module.SpawnShape = 0;
	m_Module.SpawnShapeScale.x = 0;
	m_Module.SpawnShapeScale.y = 0;

	m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
	m_Module.BlockSpawnShapeScale.x = 0;
	m_Module.BlockSpawnShapeScale.y = 0;

	m_Module.SpaceType = 0; // Local Space 

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 3;
	m_Module.AddVelocityFixedDir = Vec3(0.1f, 0.1f, 0); // 이걸 내 방향 기준으로
	m_Module.AddMinSpeed = 1;
	m_Module.AddMaxSpeed = 1;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec4(0.6f, 0.2f, 0.f, 0.01f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.7f;
	m_Module.VelocityAlignment = true;

	// SPAWN BURST
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstCount = 1;
	m_Module.SpawnBurstRepeat = false;
	m_Module.SpawnBurstRepeatTime = 60.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 10.f;
	m_Module.EndScale = 1.f;

	// Drag Module (감속)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 1.f;
	m_Module.LimitSpeed = 2.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm = 0.3f;
	m_Module.NoiseForceScale = 600.f;

	return m_Module;
}