#include "pch.h"
#include "Shield_Script.h"


Shield_Script::Shield_Script()
		: CScript((UINT)SCRIPT_TYPE::SHIELD_SCRIPT)
		, pParticle(nullptr)
		, pParticle2(nullptr)
{
}

Shield_Script::~Shield_Script()
{
}

void Shield_Script::Begin()
{
}

void Shield_Script::Tick()
{
	if (KEY_TAP(KEY::KEY_2) && KEY_PRESSED(KEY::LSHIFT))
	{
		// Particle Object
		if (pParticle == nullptr)
		{
			// 파티클 기능(모듈) 정보 세팅
			// Spawn Module
			tParticleModule m_Module;

			m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
			m_Module.SpawnRate = 2;
			m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.7f);
			m_Module.MinLife = 1.f;
			m_Module.MaxLife = 1.5f;
			m_Module.vSpawnMinScale = Vec3(150.f, 150.f, 1.f);
			m_Module.vSpawnMaxScale = Vec3(150.f, 150.f, 1.f);

			m_Module.SpawnShape = 0;
			m_Module.SpawnShapeScale.x = 0.f;

			m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
			m_Module.BlockSpawnShapeScale.x = 0;
			m_Module.BlockSpawnShapeScale.y = 0;

			m_Module.SpaceType = 0; // Local Space 

			// AddVelocity Module
			m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
			m_Module.AddVelocityType = 4;
			m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
			m_Module.AddMinSpeed = 1.f;
			m_Module.AddMaxSpeed = 1.f;

			// Render Module
			m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
			m_Module.EndColor = Vec3(0.7f, 0.3f, 0.7f);
			m_Module.FadeOut = true;
			m_Module.FadeOutStartRatio = 0.01f;
			m_Module.VelocityAlignment = true;

			m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;

			// Scale Module
			m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
			m_Module.StartScale = 1.f;
			m_Module.EndScale = 1.f;

			// Drag Module (감속)
			m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
			m_Module.DestNormalizedAge = 1.f;
			m_Module.LimitSpeed = 3.f;

			// Noise Force Module
			m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
			m_Module.NoiseForceTerm = 0.3f;
			m_Module.NoiseForceScale = 1.f;

			pParticle = new CGameObject;
			pParticle->SetName(L"Particle1-1");
			pParticle->AddComponent(new CParticleSystem(m_Module, CAssetMgr::GetInst()->Load<CTexture>(L"TestTex1", L"Texture//particle//Circle23_FullyWhite.png")));
			CreateObject(pParticle, 2, false);

			AddChild(GetOwner(), pParticle);

			{
				// 파티클 기능(모듈) 정보 세팅
				// Spawn Module
				tParticleModule m_Module;

				m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
				m_Module.SpawnRate = 2;
				m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.6f);
				m_Module.MinLife = 0.1f;
				m_Module.MaxLife = 0.5f;
				m_Module.vSpawnMinScale = Vec3(160.f, 160.f, 1.f);
				m_Module.vSpawnMaxScale = Vec3(160.f, 160.f, 1.f);

				m_Module.SpawnShape = 0;
				m_Module.SpawnShapeScale.x = 0.f;

				m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
				m_Module.BlockSpawnShapeScale.x = 0;
				m_Module.BlockSpawnShapeScale.y = 0;

				m_Module.SpaceType = 0; // Local Space 

				// AddVelocity Module
				m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
				m_Module.AddVelocityType = 0;
				m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
				m_Module.AddMinSpeed = 4.3f;
				m_Module.AddMaxSpeed = 6.2f;

				// Render Module
				m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
				m_Module.EndColor = Vec3(1.f, 0.2f, 0.2f);
				m_Module.FadeOut = true;
				m_Module.FadeOutStartRatio = -0.9f;
				m_Module.VelocityAlignment = true;

				m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;

				// Scale Module
				m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
				m_Module.StartScale = 1.1f;
				m_Module.EndScale = 1.f;

				// Drag Module (감속)
				m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
				m_Module.DestNormalizedAge = 1.f;
				m_Module.LimitSpeed = 3.f;

				// Noise Force Module
				m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
				m_Module.NoiseForceTerm = 0.3f;
				m_Module.NoiseForceScale = 4.f;

				pParticle2 = new CGameObject;
				pParticle2->SetName(L"Particle1-2");
				pParticle2->AddComponent(new CParticleSystem(m_Module, CAssetMgr::GetInst()->Load<CTexture>(L"TestTex2", L"Texture//particle//Thunder_4.png")));
				CreateObject(pParticle2, 2, false);

				AddChild(GetOwner(), pParticle2);
			}
		}

		else if (pParticle != nullptr)
		{
			DestroyObject(pParticle);
			DestroyObject(pParticle2);
			pParticle = nullptr;
			pParticle2 = nullptr;
		}

	}

	if (KEY_TAP(KEY::KEY_3) && KEY_PRESSED(KEY::LSHIFT))
	{
		// Particle Object
		if (pParticle3 == nullptr)
		{
			// 파티클 기능(모듈) 정보 세팅
			// Spawn Module
			tParticleModule m_Module;

			m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
			m_Module.SpawnRate = 50;
			m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.7f);
			m_Module.MinLife = 3.f;
			m_Module.MaxLife = 4.5f;
			m_Module.vSpawnMinScale = Vec3(10.f, 10.f, 1.f);
			m_Module.vSpawnMaxScale = Vec3(10.f, 10.f, 1.f);

			m_Module.SpawnShape = 0;
			m_Module.SpawnShapeScale.x = 2000.f;

			m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
			m_Module.BlockSpawnShapeScale.x = 0;
			m_Module.BlockSpawnShapeScale.y = 0;

			m_Module.SpaceType = 0; // Local Space 

			// AddVelocity Module
			m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
			m_Module.AddVelocityType = 3;
			m_Module.AddVelocityFixedDir = Vec3(0.f, -40.f, 0.f);
			m_Module.AddMinSpeed = 100.f;
			m_Module.AddMaxSpeed = 500.f;

			// Render Module
			m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
			m_Module.EndColor = Vec3(0.7f, 0.3f, 0.7f);
			m_Module.FadeOut = true;
			m_Module.FadeOutStartRatio = 0.01f;
			m_Module.VelocityAlignment = true;

			m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;

			// Scale Module
			m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
			m_Module.StartScale = 1.f;
			m_Module.EndScale = 0.7f;

			// Drag Module (감속)
			m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
			m_Module.DestNormalizedAge = 1.f;
			m_Module.LimitSpeed = 3.f;

			// Noise Force Module
			m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
			m_Module.NoiseForceTerm = 0.3f;
			m_Module.NoiseForceScale = 200.f;

			pParticle3 = new CGameObject;
			pParticle3->SetName(L"Particle2");
			pParticle3->AddComponent(new CParticleSystem(m_Module, CAssetMgr::GetInst()->Load<CTexture>(L"TestTex3", L"Texture//particle//Glow2_FullyWhite.png")));
			pParticle3->Transform()->SetRelativePos(Vec3(300.f, 350.f, 1.f));
			CreateObject(pParticle3, 2, false);
		}

		else if (pParticle3 != nullptr)
		{
			DestroyObject(pParticle3);
			pParticle3 = nullptr;

		}
	}

	if (KEY_TAP(KEY::KEY_4) && KEY_PRESSED(KEY::LSHIFT))
	{
		// Particle Object
		if (pParticle4 == nullptr)
		{
			// 파티클 기능(모듈) 정보 세팅
			// Spawn Module
			tParticleModule m_Module;

			m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
			m_Module.SpawnRate = 50;
			m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.7f);
			m_Module.MinLife = 3.f;
			m_Module.MaxLife = 4.5f;
			m_Module.vSpawnMinScale = Vec3(10.f, 10.f, 1.f);
			m_Module.vSpawnMaxScale = Vec3(10.f, 10.f, 1.f);

			m_Module.SpawnShape = 0;
			m_Module.SpawnShapeScale.x = 2000.f;

			m_Module.BlockSpawnShape = 0; // 생성될 지형의 제한 
			m_Module.BlockSpawnShapeScale.x = 0;
			m_Module.BlockSpawnShapeScale.y = 0;

			m_Module.SpaceType = 0; // Local Space 

			// AddVelocity Module
			m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
			m_Module.AddVelocityType = 3;
			m_Module.AddVelocityFixedDir = Vec3(0.f, -40.f, 0.f);
			m_Module.AddMinSpeed = 100.f;
			m_Module.AddMaxSpeed = 500.f;

			// Render Module
			m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
			m_Module.EndColor = Vec3(0.7f, 0.3f, 0.7f);
			m_Module.FadeOut = true;
			m_Module.FadeOutStartRatio = 0.01f;
			m_Module.VelocityAlignment = true;

			m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;

			// Scale Module
			m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
			m_Module.StartScale = 1.f;
			m_Module.EndScale = 0.7f;

			// Drag Module (감속)
			m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
			m_Module.DestNormalizedAge = 1.f;
			m_Module.LimitSpeed = 3.f;

			// Noise Force Module
			m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
			m_Module.NoiseForceTerm = 0.3f;
			m_Module.NoiseForceScale = 200.f;

			pParticle4 = new CGameObject;
			pParticle4->SetName(L"Particle2");
			pParticle4->AddComponent(new CParticleSystem(m_Module, CAssetMgr::GetInst()->Load<CTexture>(L"TestTex3", L"Texture//particle//Glow2_FullyWhite.png")));
			pParticle4->Transform()->SetRelativePos(Vec3(300.f, 350.f, 1.f));
			CreateObject(pParticle4, 2, false);
		}

		else if (pParticle4 != nullptr)
		{
			DestroyObject(pParticle4);
			pParticle4 = nullptr;

		}
	}

}

void Shield_Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Shield_Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Shield_Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void Shield_Script::CreateArrow()
{
}

