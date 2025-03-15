#include "pch.h"
#include "CBoss_Nor_Atk1.h"
#include "CBoss_Ai.h"
#include "CBoss_Atk_Obj.h"

CBoss_Nor_Atk1::CBoss_Nor_Atk1()
	: CScript((UINT)SCRIPT_TYPE::BOSS_NOR_ATK1)
	, m_Atk(false)
	, pParticle(nullptr)
{
	SetAI(true);
}

CBoss_Nor_Atk1::~CBoss_Nor_Atk1()
{
}

void CBoss_Nor_Atk1::Begin()
{
	SetAI(true);
	m_Atk = false;
	pParticle = nullptr;
}

void CBoss_Nor_Atk1::AI_In()
{
	FlipbookPlayer()->Play((UINT)Boss_Ani_State::Boss_Nor_Atk1, 12.f, true);
	m_Atk = false;

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();

	bool left = CBoss_Ai::IsLeft();
	if (left)
	{
		pos.x += 140.f;
		pos.y += 10.f;
	}
	else
	{
		pos.x -= 140.f;
		pos.y += 10.f;
	}

	pParticle = new CGameObject;
	pParticle->SetName(L"Dash_Particle");
	pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Flare8_1024.png")));
	pParticle->Transform()->SetRelativePos(pos);
	CreateObject(pParticle, 2, false);

	FlipbookPlayer()->SetTerm(1, 0.1f);

	// Sound 로딩하기
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\Boss_Berserker_TripleSlash_1.wav", L"Sound\\Boss_Berserker_TripleSlash_1.wav");
	pSound->Play(1, 0.5f, true);
}

void CBoss_Nor_Atk1::AI_Tick()
{
	if (FlipbookPlayer()->GetCurIdx() == 4 && !m_Atk)
	{
		CreateAtk();
		m_Atk = true;
	}

	if (FlipbookPlayer()->isFinish())
	{
		B_CHANGE(SCRIPT_TYPE::BOSS_NOR_ATK2);
		return;
	}
}


void CBoss_Nor_Atk1::AI_Exit()
{
	DestroyObject(pParticle);
}


void CBoss_Nor_Atk1::SaveComponent(FILE* _File)
{
}

void CBoss_Nor_Atk1::LoadComponent(FILE* _File)
{
}

void CBoss_Nor_Atk1::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Nor_Atk1::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Nor_Atk1::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss_Nor_Atk1::CreateAtk()
{
	float baseScaleX = 0.0f;
	float baseScaleY = 0.0f;
	Vec3 Rota = Vec3(0.f, 0.f, 0.f);

	Vec3 Pos = GetOwner()->Transform()->GetRelativePos();

	bool left = CBoss_Ai::IsLeft(); // 보스가 왼쪽 보는지 오른쪽 보는지

	CGameObject* BossAtk = new CGameObject;
	BossAtk->AddComponent(new CMeshRender);
	BossAtk->AddComponent(new CCollider2D);
	BossAtk->AddComponent(new CFlipbookPlayer);
	BossAtk->AddComponent(new CBoss_Atk_Obj);
	BossAtk->SetName(L"B_Atk1");
	
	BossAtk->Collider2D()->SetIndependentScale(false);
	BossAtk->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

	BossAtk->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	BossAtk->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BossSkillMtrl"));
	BossAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj_First"));
	BossAtk->FlipbookPlayer()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Boss_Atk_Obj"));
	BossAtk->Transform()->SetIndependentScale(true);
	                                                            
	BossAtk->MeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 1.f, 20.f, 1.f));

	if (left) // 보스가 왼쪽을 보고있다면
	{
		Pos.x -= 20.f;
		baseScaleX = -400.0f;
		baseScaleY = 600.0f;
		Rota = Vec3(XM_PI, XM_PI, (XM_PI/3) * 2);
	}
	else // 오른쪽을 보고 있다면
	{
		Pos.x += 20.f;
		baseScaleX = 400.0f;
		baseScaleY = 600.0f;
		Rota = Vec3(0.f,0.f, (XM_PI / 3));
	}

	BossAtk->Transform()->SetRelativePos(Pos);
	BossAtk->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	BossAtk->Transform()->SetRelativeRotation(Rota);
	

	BossAtk->FlipbookPlayer()->Play(0, 80.f, false);

	CreateObject(BossAtk, 6, false);

	//LightningffectMtrl
	//CGameObject* Lightningffect = new CGameObject;
	//Lightningffect->AddComponent(new CMeshRender);
	//Lightningffect->SetName(L"pAtkT");
	//Lightningffect->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//Lightningffect->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LightningffectMtrl"));
	//Lightningffect->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Thunder.png"));
	//Lightningffect->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
	//Lightningffect->Transform()->SetRelativePos(Pos);

	//Lightningffect->Transform()->SetIndependentScale(true);
	//CreateObject(Lightningffect, 0, false);

	//AddChild(BossAtk, Lightningffect);

	// 기본 적으로 크기 x 300 에 y 500 로테이션 z 60 이게 오른쪽 볼때 비슷한거 같고
	// 왼쪽 볼때 이거랑 똑같이 만들려면 x 에 -1 곱하고 로테이션 x , y 에 180 넣고 z 에 + 60도 해줘야함
	// 그리고 위치는 한 40 정도 더해주면 괜찮은듯

}

tParticleModule CBoss_Nor_Atk1::ParticleSetting()
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
	m_Module.BlockSpawnShapeScale.x =0;
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