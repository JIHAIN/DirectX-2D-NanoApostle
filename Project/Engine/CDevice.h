#pragma once
#include "singleton.h"

#include "CTexture.h"

class CConstBuffer;

class CDevice :
    public singleton<CDevice>
{
    SINGLE(CDevice);
private:
    HWND                            m_hWnd;         // ȭ���� �׸� ������
    Vec2                            m_Resolution;   // ������ �ػ�

    ComPtr<ID3D11Device>            m_Device;       // GPU ����, GPU �޸𸮿� �����͸� ����    
    ComPtr<ID3D11DeviceContext>     m_Context;      // GPU ����, GPU ���� ���, Rendering, GPGPU(General Purpose GPU)

    ComPtr<IDXGISwapChain>          m_SwapChain;    // BackBuffer ���� �� ���� ������ ����

    CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];
    ComPtr<ID3D11RasterizerState>   m_RSState[(UINT)RS_TYPE::END];
    ComPtr<ID3D11BlendState>        m_BSState[(UINT)BS_TYPE::END];
    ComPtr<ID3D11DepthStencilState> m_DSState[(UINT)DS_TYPE::END];

    ComPtr<ID3D11SamplerState>      m_Sampler[2];

public:
    Vec2 GetRenderResolution() { return m_Resolution; }

    ComPtr<ID3D11Device> GetDevice() { return m_Device; }
    ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }
    CConstBuffer* GetCB(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
    ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
    ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }
    ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }

    void Present();

public:
    int Init(HWND _hWnd, Vec2 _Resolution);

private:
    int CreateSwapChain();
    int CreateView();
    int CreateConstBuffer();
    int CreateRasterizerState();
    int CreateBlendState();
    int CreateDepthStencilState();
    int CreateSamplerState();
};

