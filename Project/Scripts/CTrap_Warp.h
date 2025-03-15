#pragma once
#include <Engine/CScript.h>

class CTrap_Warp :
    public CScript
{
private:
    float m_Time;
    bool  m_WarpOn;

    CGameObject* PostObj;
    void createPost();
public:
    virtual void Tick() override;
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CTrap_Warp);
    CTrap_Warp();
    ~CTrap_Warp();
};

