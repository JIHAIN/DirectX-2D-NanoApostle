#include "pch.h"
#include "CParticleTickCS.h"

#include "CStructuredBuffer.h"
#include "CAssetMgr.h"


CParticleTickCS::CParticleTickCS()
	: CComputeShader(L"Shader\\particle_tick.fx", "CS_ParticleTick", 1024, 1, 1)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_NoiseTex1(nullptr)
	, m_NoiseTex2(nullptr)
	, m_NoiseTex3(nullptr)
	, m_ParticleWorldPos(0.f,0.f,0.f)

{
	m_NoiseTex1 = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg");
	m_NoiseTex2 = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\Noise17.png", L"Texture\\noise\\Noise17.png");
	m_NoiseTex3 = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\Noise95.png", L"Texture\\noise\\Noise95.png");
}

CParticleTickCS::~CParticleTickCS()
{
}

int CParticleTickCS::Binding()
{
	if (nullptr == m_ParticleBuffer || nullptr == m_SpawnCountBuffer)
		return E_FAIL;

	// 파티클 버퍼 u0 에 바인딩
	m_ParticleBuffer->Binding_CS_UAV(0);

	// 스폰 가능 개수 u1 에 바인딩
	m_SpawnCountBuffer->Binding_CS_UAV(1);

	// Noise Texture t20, t22, t23 바인딩
	m_NoiseTex1->Binding_SRV_CS(20);
	m_NoiseTex2->Binding_SRV_CS(22);
	m_NoiseTex3->Binding_SRV_CS(23);

	// 모듈 데이터 t21 에 바인딩
	m_ModuleBuffer->Binding_CS_SRV(21);


	// 파티클 요소 개수(파티클 수)
	m_Const.iArr[0] = (int)m_ParticleBuffer->GetElementCount();

	// 파티클 게임 오브젝트 중심 위치
	m_Const.v4Arr[0] = m_ParticleWorldPos;

	return S_OK;
}

void CParticleTickCS::CalcGroupCount()
{
	m_GroupX = m_ParticleBuffer->GetElementCount() / m_GroupPerThreadX;
	if (m_ParticleBuffer->GetElementCount() % m_GroupPerThreadX)
		m_GroupX += 1;

	m_GroupY = 1;
	m_GroupZ = 1;
}

void CParticleTickCS::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV(0);
	m_ParticleBuffer = nullptr;

	m_SpawnCountBuffer->Clear_CS_UAV(1);
	m_SpawnCountBuffer = nullptr;

	m_NoiseTex1->Clear_SRV_CS();
	m_NoiseTex2->Clear_SRV_CS();
	m_NoiseTex3->Clear_SRV_CS();

	m_ModuleBuffer->Clear_CS_SRV(21);
	m_ModuleBuffer = nullptr;
}

