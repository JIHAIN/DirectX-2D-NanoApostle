#include "pch.h"
#include "CShield_Script.h"
#include "CPlayer_Ai.h"
#include "CShield_Script_Obj.h"

CShield_Script::CShield_Script()
	: CScript((UINT)SCRIPT_TYPE::SHIELD_SCRIPT)
	, pParticle(nullptr)
	, m_Shield(nullptr)
	, MousDir(Vec3(0.f, 0.f, 0.f))
	, m_CreateShield_Time(0.f)
	, m_Parrying(true)
	, m_ParryingTime(0.5f)
{
	SetAI(true);
}

CShield_Script::~CShield_Script()
{
}



void CShield_Script::Begin()
{
	SetAI(true);
	m_Shield = nullptr;
	pParticle = nullptr;
	MousDir = Vec3(0.f, 0.f, 0.f);
	m_CreateShield_Time = 0.f;

	m_Parrying = false;
	m_ParryingTime = 0.5f;
}

void CShield_Script::AI_In()
{
	Vec3 MousePos = g_MouseInfo.MouseWorldPos;
	Vec3 pWPos = Transform()->GetWorldPos();
	Vec3 PlayerMouseDir = MousePos - pWPos;
	MousDir = PlayerMouseDir.Normalize();

	PlayerAniPlay(MousDir);
	CreateShield();

	// Sound 로딩하기
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\Block.wav", L"Sound\\Block.wav");
	pSound->Play(1, 0.6f, true);

	m_Parrying = true;

	m_ParryingTime = 0.3f;
}

void CShield_Script::AI_Tick()
{
	m_ParryingTime -= DT;

	if (m_ParryingTime <= 0.f)
		m_Parrying = false;

	if (KEY_RELEASED(KEY::RBTN))
	{
		P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
		return;
	}
	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		P_CHANGE(SCRIPT_TYPE::PLAYER_DASH);
		return;
	}
}


void CShield_Script::AI_Exit()
{
	DestroyObject(m_Shield);
	DestroyObject(pParticle);
	m_Parrying = false;
}

void CShield_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CShield_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CShield_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}



void CShield_Script::Tick()
{
}


void CShield_Script::SaveComponent(FILE* _File)
{
}

void CShield_Script::LoadComponent(FILE* _File)
{
}


tParticleModule CShield_Script::ParticleSetting()
{
	// 파티클 기능(모듈) 정보 세팅
			// Spawn Module
	tParticleModule m_Module;

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate = 10;
	m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Module.MinLife = 0.4f;
	m_Module.MaxLife = 1.0f;
	m_Module.vSpawnMinScale = Vec3(10.f, 10.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(20.f, 20.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale.x = 100.f;
	m_Module.SpawnShapeScale.y = 70.f;

	m_Module.BlockSpawnShape = 1; // 생성될 지형의 제한 
	m_Module.BlockSpawnShapeScale.x = 100;
	m_Module.BlockSpawnShapeScale.y = 90;

	m_Module.SpaceType = 0; // Local Space 

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 1;
	m_Module.AddVelocityFixedDir = Vec3(10.f, 10.f, 0.f);
	m_Module.AddMinSpeed = 10.f;
	m_Module.AddMaxSpeed = 30.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec4(1.f, 1.f, 1.f, 0.1f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.01f;
	m_Module.VelocityAlignment = false;

	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstCount = 20.f;
	m_Module.SpawnBurstRepeat = false;
	m_Module.SpawnBurstRepeatTime = 40.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 2.f;
	m_Module.EndScale = 0.5f;

	// Drag Module (감속)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_Module.DestNormalizedAge = 1.f;
	m_Module.LimitSpeed = 3.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
	m_Module.NoiseForceTerm = 0.6f;
	m_Module.NoiseForceScale = 100.f;

	return m_Module;
}

void CShield_Script::PlayerAniPlay(Vec3 _Dir)
{
	float angle = atan2(_Dir.x, _Dir.y) * 180.0f / XM_PI;

	// 방향 판정
	if (angle >= -45 && angle < 45)// 위쪽 (Up)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Shield_Up, 10.f, false);
	}
	else if (angle >= 45 && angle < 135)// 오른쪽 (Right)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Shield_Right, 10.f, false);
	}
	else if (angle >= -135 && angle < -45) // 왼쪽 (Left)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Shield_Right, 10.f, false);
	}
	else // 아래쪽 (Down)
	{
		FlipbookPlayer()->Play((UINT)Player_Ani_State::Shield_Down, 10.f, false);
	}

}

void CShield_Script::CreateShield()
{
	Vec3 pPos = GetOwner()->Transform()->GetRelativePos();

	m_Shield = new CGameObject;
	m_Shield->AddComponent(new CMeshRender);
	m_Shield->AddComponent(new CCollider2D);
	m_Shield->AddComponent(new CLight2D);
	m_Shield->AddComponent(new CShield_Script_Obj);
	m_Shield->SetName(L"Shield");
	
	m_Shield->Light2D()->SetLightColor(Vec3(0.1f, 0.1f, 0.1f));
	m_Shield->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	m_Shield->Light2D()->SetRadius(50.f);
	
	m_Shield->Collider2D()->SetIndependentScale(false);
	m_Shield->Collider2D()->SetScale(Vec2(1.f, 1.f));
	
	m_CreateShield_Time = g_Data.Time;

	m_Shield->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Shield->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShieldMtrl"));
	m_Shield->MeshRender()->CreateDynamicMaterial();
	m_Shield->MeshRender()->GetMaterial()->SetTexParam(TEX_0,CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\particle\\Shield_big.png", L"Texture\\particle\\Shield_big.png"));
	m_Shield->MeshRender()->GetMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\Noise89.png", L"Texture\\noise\\Noise89.png"));
	m_Shield->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_CreateShield_Time);

	m_Shield->Transform()->SetRelativePos(pPos);
	m_Shield->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 1.f));
	m_Shield->Transform()->SetIndependentScale(true);
	CreateObject(m_Shield, 4, false);

	pPos.y += 20.f;

	pParticle = new CGameObject;
	pParticle->SetName(L"Particle");
	pParticle->AddComponent(new CParticleSystem(ParticleSetting(), CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\particle\\Shield_particle2.png")));
	pParticle->Transform()->SetRelativePos(pPos);
	CreateObject(pParticle, 2, false);

	/*m_Shield->SetLayerIdx(4);
	AddChild(GetOwner(), m_Shield);*/
}
