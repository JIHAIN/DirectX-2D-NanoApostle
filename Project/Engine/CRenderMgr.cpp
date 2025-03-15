#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight2D.h"

#include "CStructuredBuffer.h"

#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"

CRenderMgr::CRenderMgr()
	: m_DbgObj(nullptr)
	, m_EditorCam(nullptr)
	, m_Light2DBuffer(nullptr)	
	, m_IsEditor(false)
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLight2DInfo), 2, SB_TYPE::SRV_ONLY, true);
}

CRenderMgr::~CRenderMgr()
{
	DELETE(m_DbgObj);
	DELETE(m_Light2DBuffer);
}

void CRenderMgr::Init()
{
	// DebugRender 용 DummyObject
	m_DbgObj = new CGameObject;
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
	
	// PostProcess 용 텍스쳐
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
}

void CRenderMgr::Render()
{
	// 렌더링 시작
	RenderStart();
	
	if (m_IsEditor)
	{
		Render_Editor();
	}	
	else
	{
		Render_Play();
	}

	// DebugRender
	Render_Debug();	
}

void CRenderMgr::RenderStart()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

	// Target Clear
	Vec4 vClearColor = Vec4(0.1f, 0.1f, 0.1f, 0.f);
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), vClearColor);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// ViewPort 설정 - Window 화면 영역 지정
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	viewport.Width = vResolution.x;
	viewport.Height = vResolution.y;

	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);

	// Rendering 목적지를 지정
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());


	// 데이터 및 리소스 바인딩
	Binding();
}

void CRenderMgr::Binding()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);

	g_Data.Light2DCount = (int)m_vecLight2D.size();

	pCB->SetData(&g_Data);
	pCB->Binding();
	pCB->Binding_CS(); // 상수버퍼 바인딩 컴퓨트쉐이더에도 해줌 

	// 2D 광원 정보 바인딩
	static vector<tLight2DInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLight2DInfo());
	}

	// 데이터를 받을 구조화버퍼의 크기가 모자라면, Resize 한다.
	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	// 광원 1개 이상인 경우 구조화 버퍼로 데이터 이동
	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), vecLight2DInfo.size());
		m_Light2DBuffer->Binding(13);
	}

	m_vecLight2D.clear();
	vecLight2DInfo.clear();
}

void CRenderMgr::Render_Debug()
{
	list<tDebugShapeInfo>::iterator iter = m_DbgList.begin();

	for (; iter != m_DbgList.end(); )
	{
		// 모양 설정
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			break;
		case DEBUG_SHAPE::LINE:
			break;
		case DEBUG_SHAPE::MOUSE:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));

			break;		
		}

		// 위치설정
		if ((*iter).matWorld == XMMatrixIdentity())
		{
			m_DbgObj->Transform()->SetRelativePos((*iter).WorldPos);
			m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
			m_DbgObj->Transform()->SetRelativeRotation((*iter).Rotation);
			m_DbgObj->Transform()->FinalTick();
		}
		else
		{
			m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
		}
				
		// 마우스 일때만 다르게 그려주도록 설정
		if (DEBUG_SHAPE::MOUSE == (*iter).Shape)
		{
			if ((*iter).matWorld == XMMatrixIdentity())
			{
				// 스케일의 절반만큼 위치를 조정합니다.
				XMFLOAT3 adjustedPos = (*iter).WorldPos;
				adjustedPos.x += (*iter).Scale.x * 0.5f;
				adjustedPos.y -= (*iter).Scale.y * 0.5f;

				m_DbgObj->Transform()->SetRelativePos(adjustedPos);
				m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
				m_DbgObj->Transform()->SetRelativeRotation((*iter).Rotation);
				m_DbgObj->Transform()->FinalTick();
			}
		}
		else
		{
			// 위치설정
			if ((*iter).matWorld == XMMatrixIdentity())
			{
				m_DbgObj->Transform()->SetRelativePos((*iter).WorldPos);
				m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
				m_DbgObj->Transform()->SetRelativeRotation((*iter).Rotation);
				m_DbgObj->Transform()->FinalTick();
			}
			else
			{
				m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
			}
		}

		// 색상 설정
		m_DbgObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);

		if ((*iter).DepthTest)
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_WRITE);
		}
		else
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
		}

		// 렌더
		m_DbgObj->Render();

		(*iter).Time += DT;
		if ((*iter).Duration <= (*iter).Time)
		{
			iter = m_DbgList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::Render_Play()
{
	// 레벨 내에 카메라로 레벨 렌더링
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (m_vecCam[i] != nullptr)
		{
			m_vecCam[i]->SortObject();
			m_vecCam[i]->Render();
		}
	}
}

void CRenderMgr::Render_Editor()
{
	m_EditorCam->SortObject();
	m_EditorCam->Render();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, UINT _Priority)
{
	if (-1 == _Priority)
	{
	/*	vector<CCamera*>::iterator iter = m_vecCam.begin();
		for (; iter !=  m_vecCam.end(); ++iter)
		{
			if (*iter == _Cam)
			{
				m_vecCam.erase(iter);
				return;
			}
		}*/
	}

	else
	{
		if (m_vecCam.size() <= _Priority)
		{
			m_vecCam.resize(_Priority + 1);
		}		

		//assert(!m_vecCam[_Priority]);

		m_vecCam[_Priority] = _Cam;
	}
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::ChangeCamera(CCamera* _Cam, UINT _Priority)
{
	// 카메라를 재 등록할때 카메라의 원래 위치를 지워주고 새로 설정한 _Priority에 다시 등록한다.
	// 지웠으면 이제 설정한 위치로 넣어줘야 하는데 이때 내가 가려는 자리가 nullptr이 아니라면
	// 상대와 나의 위치를 바꿔준다.
	// 지우면 크기가 변하니까 지우지 말고 nullptr 넣어줌

	int TempPrior = _Cam->GetCameraPriority(); // 원래 내 우선순위 받아둠.

	if (_Priority == -1)
	{
		if (TempPrior == -1)
			return;

		m_vecCam[TempPrior] = nullptr;
		_Cam->SetPriority(_Priority);
	}
	else // 우선순위가 -1이 아니면 해야할일 해줌
	{
		if (m_vecCam.size() <= _Priority) // 원래 크기보다 큰 값이 들어오면 새로 할당한다.
		{
			m_vecCam.resize(_Priority + 1);
		}

		// 먼저 지워주기 전에 그 자리에 값이 있는지 없는지를 확인
		if (m_vecCam[_Priority] && TempPrior != -1) // nullptr이 아니고 
		{
			CCamera* TempCam = m_vecCam[_Priority]; //임시로 원래자리에 있던 카메라 받아두고

			m_vecCam[TempPrior] = TempCam;   // 나랑 자리 바꿔주고
			TempCam->SetPriority(TempPrior); // 새로 순위도 설정해주고

			_Cam->SetPriority(_Priority); // 우선도 세팅
			m_vecCam[_Priority] = _Cam;   // 데이터 넣어주기
		}
		else if (m_vecCam[_Priority] && TempPrior == -1) // nullptr이 아니고 내가 -1 에서 0으로 올라갈때면 0을 -1로 보내야 하니까 그 자리를 그냥 nullptr로 막아버림
		{
			CCamera* TempCam = m_vecCam[_Priority];
			TempCam->SetPriority(TempPrior); // 새로 순위도 설정해주고

			m_vecCam[_Priority] = nullptr; //자리에 그냥 널포인터 넣어버리고

			_Cam->SetPriority(_Priority); // 우선도 세팅
			RegisterCamera(_Cam, _Priority);   // 데이터 넣어주기
		}

		if (!m_vecCam[_Priority] && TempPrior != -1)	//가려는 자리가 nullptr 이면 그냥 넣고 전에 자리를 지우는게 아니라 nullptr을 넣어줌 내가 -1 에서 올라오는게 아니여야함
		{
			m_vecCam[TempPrior] = nullptr; // 전에 자리에 nullptr 넣기
			_Cam->SetPriority(_Priority); // 우선도 세팅
			m_vecCam[_Priority] = _Cam;   // 데이터 넣어주기
		}
		else if (!m_vecCam[_Priority] && TempPrior == -1) // 내가 -1 에서 올라오는 거라면
		{
			_Cam->SetPriority(_Priority);
			RegisterCamera(_Cam, _Priority);
		}
	}
}
