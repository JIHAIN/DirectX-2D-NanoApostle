#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_Mesh;

    Ptr<CMaterial>          m_SharedMtrl;  // ����
    Ptr<CMaterial>          m_CurMtrl;     // ���� ������� ����
    Ptr<CMaterial>          m_DynamicMtrl; // ���� ����(�ӽ�)


public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);
    void SetMesh(wstring _MeshName) { m_Mesh = CAssetMgr::GetInst()->FindAsset<CMesh>(_MeshName); }
    void SetMaterial(wstring _MaterName);
    Ptr<CMesh> GetMesh() { return m_Mesh; }


    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetMaterial() { return m_CurMtrl; }
    Ptr<CMaterial> GetDynamicMaterial();
    void CreateDynamicMaterial();


public:
    virtual void Render() = 0;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;


public:
    virtual CRenderComponent* Clone() = 0;

    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    ~CRenderComponent();
};

