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
	// DebugRender �� DummyObject
	m_DbgObj = new CGameObject;
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
	
	// PostProcess �� �ؽ���
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
}

void CRenderMgr::Render()
{
	// ������ ����
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

	// ViewPort ���� - Window ȭ�� ���� ����
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	viewport.Width = vResolution.x;
	viewport.Height = vResolution.y;

	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);

	// Rendering �������� ����
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());


	// ������ �� ���ҽ� ���ε�
	Binding();
}

void CRenderMgr::Binding()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);

	g_Data.Light2DCount = (int)m_vecLight2D.size();

	pCB->SetData(&g_Data);
	pCB->Binding();
	pCB->Binding_CS(); // ������� ���ε� ��ǻƮ���̴����� ���� 

	// 2D ���� ���� ���ε�
	static vector<tLight2DInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLight2DInfo());
	}

	// �����͸� ���� ����ȭ������ ũ�Ⱑ ���ڶ��, Resize �Ѵ�.
	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	// ���� 1�� �̻��� ��� ����ȭ ���۷� ������ �̵�
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
		// ��� ����
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

		// ��ġ����
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
				
		// ���콺 �϶��� �ٸ��� �׷��ֵ��� ����
		if (DEBUG_SHAPE::MOUSE == (*iter).Shape)
		{
			if ((*iter).matWorld == XMMatrixIdentity())
			{
				// �������� ���ݸ�ŭ ��ġ�� �����մϴ�.
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
			// ��ġ����
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

		// ���� ����
		m_DbgObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);

		if ((*iter).DepthTest)
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_WRITE);
		}
		else
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
		}

		// ����
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
	// ���� ���� ī�޶�� ���� ������
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
	// ī�޶� �� ����Ҷ� ī�޶��� ���� ��ġ�� �����ְ� ���� ������ _Priority�� �ٽ� ����Ѵ�.
	// �������� ���� ������ ��ġ�� �־���� �ϴµ� �̶� ���� ������ �ڸ��� nullptr�� �ƴ϶��
	// ���� ���� ��ġ�� �ٲ��ش�.
	// ����� ũ�Ⱑ ���ϴϱ� ������ ���� nullptr �־���

	int TempPrior = _Cam->GetCameraPriority(); // ���� �� �켱���� �޾Ƶ�.

	if (_Priority == -1)
	{
		if (TempPrior == -1)
			return;

		m_vecCam[TempPrior] = nullptr;
		_Cam->SetPriority(_Priority);
	}
	else // �켱������ -1�� �ƴϸ� �ؾ����� ����
	{
		if (m_vecCam.size() <= _Priority) // ���� ũ�⺸�� ū ���� ������ ���� �Ҵ��Ѵ�.
		{
			m_vecCam.resize(_Priority + 1);
		}

		// ���� �����ֱ� ���� �� �ڸ��� ���� �ִ��� �������� Ȯ��
		if (m_vecCam[_Priority] && TempPrior != -1) // nullptr�� �ƴϰ� 
		{
			CCamera* TempCam = m_vecCam[_Priority]; //�ӽ÷� �����ڸ��� �ִ� ī�޶� �޾Ƶΰ�

			m_vecCam[TempPrior] = TempCam;   // ���� �ڸ� �ٲ��ְ�
			TempCam->SetPriority(TempPrior); // ���� ������ �������ְ�

			_Cam->SetPriority(_Priority); // �켱�� ����
			m_vecCam[_Priority] = _Cam;   // ������ �־��ֱ�
		}
		else if (m_vecCam[_Priority] && TempPrior == -1) // nullptr�� �ƴϰ� ���� -1 ���� 0���� �ö󰥶��� 0�� -1�� ������ �ϴϱ� �� �ڸ��� �׳� nullptr�� ���ƹ���
		{
			CCamera* TempCam = m_vecCam[_Priority];
			TempCam->SetPriority(TempPrior); // ���� ������ �������ְ�

			m_vecCam[_Priority] = nullptr; //�ڸ��� �׳� �������� �־������

			_Cam->SetPriority(_Priority); // �켱�� ����
			RegisterCamera(_Cam, _Priority);   // ������ �־��ֱ�
		}

		if (!m_vecCam[_Priority] && TempPrior != -1)	//������ �ڸ��� nullptr �̸� �׳� �ְ� ���� �ڸ��� ����°� �ƴ϶� nullptr�� �־��� ���� -1 ���� �ö���°� �ƴϿ�����
		{
			m_vecCam[TempPrior] = nullptr; // ���� �ڸ��� nullptr �ֱ�
			_Cam->SetPriority(_Priority); // �켱�� ����
			m_vecCam[_Priority] = _Cam;   // ������ �־��ֱ�
		}
		else if (!m_vecCam[_Priority] && TempPrior == -1) // ���� -1 ���� �ö���� �Ŷ��
		{
			_Cam->SetPriority(_Priority);
			RegisterCamera(_Cam, _Priority);
		}
	}
}
