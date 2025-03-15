#include "pch.h"
#include "CAssetMgr.h"

#include "CDevice.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineGraphicShader();
	CreateEngineComputeShader();
	CreateEngineMaterial();
	CreateEngineSprite();
}

void CAssetMgr::CreateEngineMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	// =========
	// PointMesh
	// =========
	Vtx v;
	UINT i = 0;

	pMesh = new CMesh;
	pMesh->Create(&v, 1, &i, 1);
	CAssetMgr::GetInst()->AddAsset(L"PointMesh", pMesh);

	// =========

	// =========
	// Rect Mesh
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// =========	
	Vtx arrVtx[4] = {};
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);
	arrVtx[2].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 1.f, 1.f);

	// 인덱스
	UINT Idx[6] = { 0 , 1 , 2, 0 , 2 , 3 };

	// 클래스 or 구조체 + 가상함수
	const type_info& info = typeid(CMesh);
	const char* pName = info.name();

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, Idx, 6);
	CAssetMgr::GetInst()->AddAsset(L"RectMesh", pMesh);

	// ==============
	// RectMesh_Debug
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// ==============
	UINT DebugRectIdx[5] = { 0 , 1 , 2, 3 , 0 };

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, DebugRectIdx, 5);
	CAssetMgr::GetInst()->AddAsset(L"RectMesh_Debug", pMesh);


	// ==========
	// CircleMesh
	// ==========
	vector<Vtx>		vecVtx;
	vector<UINT>	vecIdx;

	UINT Slice = 40;
	float Radius = 0.5f;
	float Theta = 0.f;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	for (UINT i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);

		Theta += (XM_PI * 2.f) / Slice;
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i + 2);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CAssetMgr::GetInst()->AddAsset(L"CircleMesh", pMesh);

	// =================
	// CircleMesh_Debug
	// =================	
	vecIdx.clear();
	for (size_t i = 0; i < vecVtx.size() - 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CAssetMgr::GetInst()->AddAsset(L"CircleMesh_Debug", pMesh);
}

void CAssetMgr::CreateEngineTexture()
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// PostPorcessTexture
	CAssetMgr::GetInst()->CreateTexture(L"PostProcessTex", vResolution.x, vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	// NoiseTexture
	Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");

	CAssetMgr::GetInst()->Load<CTexture>(L"pRoar", L"Texture\\Roar.png");

	Ptr<CTexture> pEffectTarget = CreateTexture(
		L"EffectTargetTex"
		, (UINT)(vResolution.x), (UINT)(vResolution.y)
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	Ptr<CTexture> pEffectDepth = CreateTexture(
		L"EffectDepthStencilTex"
		, (UINT)(vResolution.x), (UINT)(vResolution.y)
		, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	Ptr<CTexture> pEffectBlurTarget = CreateTexture(
		L"EffectBlurTargetTex"
		, (UINT)(vResolution.x), (UINT)(vResolution.y)
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

}

void CAssetMgr::CreateEngineGraphicShader()
{
	Ptr<CGraphicShader> pShader = nullptr;

	// ===========================
	// Std2DShader : 표준 2D 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam("Test Parameter", VEC2_2, true);
	pShader->AddTexParam("Output Texture", TEX_0);

	CAssetMgr::GetInst()->AddAsset(L"Std2DShader", pShader);


	// ================================================
	// Std2DAlphaBlendShader : 표준 알파블렌드 2D 쉐이더
	// ================================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	//pShader->SetDSState(DS_TYPE::NO_TEST);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	CAssetMgr::GetInst()->AddAsset(L"Std2DAlphaBlendShader", pShader);

	// ===========================
	// BossShader : 보스 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Boss");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam("Test Parameter", VEC2_2, true);
	pShader->AddTexParam("Output Texture", TEX_0);

	CAssetMgr::GetInst()->AddAsset(L"BossShader", pShader);

	// ================================================
	// SkillShader : 플레이어 공격
	// ================================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_Atk");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"SkillShader", pShader);

	// ================================================
	// BossSkillShader : 보스 공격
	// ================================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_BossAtk");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"BossSkillShader", pShader);


	// =================================
	// Std2D_PaperBurn : PaperBurn 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_PaperBurn");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	CAssetMgr::GetInst()->AddAsset(L"Std2DPaperBurnShader", pShader);

	
	// =================================
	// Std2D_Tower : 맵에 타워용 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_Tower");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"TowerShader", pShader);

	// ==================================
	// TileMapShader : 타일맵 전용 쉐이더
	// ==================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"Shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::LESS);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	CAssetMgr::GetInst()->AddAsset(L"TileMapShader", pShader);

	// ==================================
	// CRepeatSprite : CRepeatSprite 전용 쉐이더
	// ==================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap.fx", "VS_RepeatSprite");
	pShader->CreatePixelShader(L"Shader\\tilemap.fx", "PS_RepeatSprite");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	CAssetMgr::GetInst()->AddAsset(L"RepeatSpriteShader", pShader);

	// ==================================
	// CRepeatSprite : CRepeatSprite 전용 이펙트 버전 쉐이더
	// ==================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap.fx", "VS_RepeatSprite");
	pShader->CreatePixelShader(L"Shader\\tilemap.fx", "PS_RepeatSprite");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"RepeatSpriteEffectShader", pShader);

	// ============
	// PostProcess
	// ============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Post");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"PostProcessShader", pShader);


	// ===========
	// Distortion
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Distortion");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"DistortionShader", pShader);

	// ========
	// Vortex
	// ========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Vortex");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Vortex");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"VortexShader", pShader);

	// ========
	// Roar
	// ========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Roar");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Roar");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"RoarShader", pShader);

	// ========
	// ShockWave
	// ========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_ShockWave");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_ShockWave");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"ShockWaveShader", pShader);

	// =======
	// HitEffect
	// =========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_ShockWave");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_HitEffect");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"HitEffectShader", pShader);

	// =======
	// FadeOutEffectShader
	// =========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_FadeOut");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"FadeOutEffectShader", pShader);

	// =======
	// FadeInEffectShader
	// =========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_FadeIn");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"FadeInEffectShader", pShader);

	// =======
	// SpreadEffectShader
	// =========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_NoiseSpread");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"SpreadEffectShader", pShader);
	// =======
	// VS_GameOver
	// =========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_GameOver");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_GameOver");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"GameOverShader", pShader);
	
	// ==================
	// 스파이크 함정 쉐이더
	// ==================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\SpikEffect.fx", "VS_SpikEffect");
	pShader->CreatePixelShader(L"Shader\\SpikEffect.fx", "PS_SpikEffect");
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"SpikEffectShader", pShader);

	// ==================
	// 번개 이펙트 쉐이더
	// ==================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\SpikEffect.fx", "VS_Lightningffect");
	pShader->CreatePixelShader(L"Shader\\SpikEffect.fx", "PS_Lightningffect");
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"LightningffectShader", pShader);
	

	// ================
	// TrapSircle 쉐이더
	// ================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\SpikEffect.fx", "VS_TrapSircle");
	pShader->CreatePixelShader(L"Shader\\SpikEffect.fx", "PS_TrapSircle");

	pShader->SetBSState(BS_TYPE::ALPHABLEND); 
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"TrapSircleShader", pShader);
	
	//===================================
	//플레이어 효과들 플레이어를 가리면 안되는것들
	//===================================

	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_Mark");

	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"PlayerEffectShader", pShader);

	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_Ring");

	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	CAssetMgr::GetInst()->AddAsset(L"PlayerRingShader", pShader);

	//==============
	// Mouse
	// =============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_Mouse");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"MouseShader", pShader);

	//==============
	// Shield
	// =============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\Player_Shield.fx", "VS_Player_Shield");
	pShader->CreatePixelShader(L"Shader\\Player_Shield.fx", "PS_Player_Shield");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	CAssetMgr::GetInst()->AddAsset(L"ShieldShader", pShader);


	// =================================
	// DebugShapeShader : 디버그용 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"Shader\\debug.fx", "PS_DebugShape");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	CAssetMgr::GetInst()->AddAsset(L"DebugShapeShader", pShader);

	// ==============
	// MyParticleShader 내가 만든버전 
	// ==============
	//pShader = new CGraphicShader;
	//pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle");
	//pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle");
	//pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_ShieldParticle");

	//pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//pShader->SetRSState(RS_TYPE::CULL_NONE);
	//pShader->SetBSState(BS_TYPE::ALPHABLEND);
	//pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);

	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	//CAssetMgr::GetInst()->AddAsset(L"MyParticleShader", pShader);

	// ==============
	// ParticleShader
	// ==============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);
	CAssetMgr::GetInst()->AddAsset(L"ParticleShader", pShader);

	// ============
	// EffectShader
	// ============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);
	AddAsset(L"EffectShader", pShader);

	// ==========
	// BlurShader
	// ==========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Blur");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Blur");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"BlurShader", pShader);

	// ===========
	// EffectMerge
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_EffectMerge");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_EffectMerge");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"EffectMergeShader", pShader);
}

#include "CParticleTickCS.h"

void CAssetMgr::CreateEngineComputeShader()
{
	AddAsset<CComputeShader>(L"ParticleTickCS", new CParticleTickCS);
}


void CAssetMgr::CreateEngineMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// BossMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"BossMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BossShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"Std2DAlphaBlendMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// SkillMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"SkillMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SkillShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// BossSkillMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"BossSkillMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BossSkillShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
	
	// PlayerEffectMtrl 페이스마커
	pMtrl = new CMaterial;
	pMtrl->SetName(L"PlayerEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PlayerEffectShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// PlayerEffectMtrl 바닥 링
	pMtrl = new CMaterial;
	pMtrl->SetName(L"PlayerRingMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PlayerRingShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\Player_SircleRing.png", L"Texture\\Player_SircleRing.png"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DPaperBurnMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"Std2DPaperBurnMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DPaperBurnShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// TileMapMaterial
	pMtrl = new CMaterial;
	pMtrl->SetName(L"TileMapMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//TowerMaterial
	pMtrl = new CMaterial;
	pMtrl->SetName(L"TowerMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TowerShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// CRepeatSpriteMaterial
	pMtrl = new CMaterial;
	pMtrl->SetName(L"RepeatSpriteMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RepeatSpriteShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
	
	// RepeatSpriteEffectMaterial
	pMtrl = new CMaterial;
	pMtrl->SetName(L"RepeatSpriteEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RepeatSpriteEffectShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// PostProcessMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"PostProcessMtrl");
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PostProcessShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"DistortionMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// VortexMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"VortexMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"VortexShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// RoarMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"RoarMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RoarShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"pRoar"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//ShockWave
	pMtrl = new CMaterial;
	pMtrl->SetName(L"ShockWaveMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ShockWaveShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//HitEffect
	pMtrl = new CMaterial;
	pMtrl->SetName(L"HitEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"HitEffectShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//FadeOutEffectMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"FadeOutEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"FadeOutEffectShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//FadeInEffectMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"FadeInEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"FadeInEffectShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//SpreadEffectMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"SpreadEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SpreadEffectShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
	


	//GameOverShader
	pMtrl = new CMaterial;
	pMtrl->SetName(L"GameOverMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"GameOverShader"));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
	
	//Mouse
	pMtrl = new CMaterial;
	pMtrl->SetName(L"MouseMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MouseShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	//ShieldMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"ShieldMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ShieldShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
	
	//DebugShapeMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"DebugShapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// MyParticleMtrl 	
	pMtrl = new CMaterial;
	pMtrl->SetName(L"MyParticleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MyParticleShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// ParticleMtrl 	
	pMtrl = new CMaterial;
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// L"Material\\Default Material %d.mtrl"
	pMtrl = new CMaterial;
	pMtrl->SetName(L"Material\\Default Material 0.mtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"HitEffectShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// 스파이크 함정 SpikEffectShader
	pMtrl = new CMaterial;
	pMtrl->SetName(L"SpikEffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SpikEffectShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// 번개 이펙트 LightningffectShader
	pMtrl = new CMaterial;
	pMtrl->SetName(L"LightningffectMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"LightningffectShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// 서클 함정 TrapSircleShader
	pMtrl = new CMaterial;
	pMtrl->SetName(L"TrapSircleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TrapSircleShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// EffectMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectShader"));
	AddAsset(L"EffectMtrl", pMtrl);

	// BlurMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	AddAsset(L"BlurMtrl", pMtrl);

	// EffectMergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectMergeShader"));
	AddAsset(L"EffectMergeMtrl", pMtrl);
	
}

void CAssetMgr::CreateEngineSprite()
{

	Ptr<CTexture> AESlash = Load<CTexture>(L"AESlash", L"Texture\\AESlash_11.png");

	Ptr<CFlipbook> AESlashFlipbook = new CFlipbook;
	Ptr<CSprite> AESlashSprite = nullptr;

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			AESlashSprite = new CSprite;
			AESlashSprite->SetAtlasTexture(AESlash);
			AESlashSprite->SetLeftTop(Vec2(j * 270.f, i * 270.f));
			AESlashSprite->SetSlice(Vec2(270.f, 270.f));
			AESlashSprite->SetBackground(Vec2(300.f, 300.f));

			wchar_t szKey[255] = {};
			swprintf_s(szKey, 255, L"AESlash%d", i + j);

			// Sprite 등록
			AddAsset(szKey, AESlashSprite);

			// flipbook 에 Sprite 추가
			AESlashFlipbook->AddSprite(AESlashSprite);
		}
	}

	// flipbook 등록
	AddAsset(L"PlayerAtk", AESlashFlipbook);

	//////////////////////
	// CG
	//////////////////////
	Ptr<CFlipbook> pFlipbookCG = new CFlipbook;
	Ptr<CSprite> pSpriteCG = nullptr;

	for (int i = 0; i < 25; ++i)
	{
		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"Texture\\CG\\CG_Memory_%d.png", i);

		Ptr<CTexture> Tex = Load<CTexture>(szKey , szKey);
		UINT Width = Tex->GetWidth();
		UINT Height = Tex->GetHeight();

		pSpriteCG = new CSprite;
		pSpriteCG->SetAtlasTexture(Tex);

		pSpriteCG->SetLeftTop(Vec2(0.f, 0.f));
		pSpriteCG->SetSlice(Vec2(Width, Height));
		pSpriteCG->SetBackground(Vec2(Width, Height));

		// Sprite 등록
		AddAsset(szKey, pSpriteCG);

		// flipbook 에 Sprite 추가
		pFlipbookCG->AddSprite(pSpriteCG);
	}

	// flipbook 등록
	AddAsset(L"CG", pFlipbookCG);

}