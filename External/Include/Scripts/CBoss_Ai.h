#pragma once
#include <Engine/CScript.h>

class CGameObject;
class CPlayer_Ai;
class CPlayer_Death;

class CBoss_Ai :
    public CScript
{
private:
    CScript*            m_CurScript; // ���� Ai Tick�� �� ��ũ��Ʈ
    CGameObject*        m_TargetPlayer;    // Ÿ���÷��̾�
    CPlayer_Ai*         m_PlayerAi;
    static bool         m_Left;

public:
    virtual void Tick() override;
    virtual void Begin() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    static bool IsLeft() { return m_Left; }

    void ChangeAi(SCRIPT_TYPE _Type);

    CGameObject* GetTargetPlayer() { return m_TargetPlayer; }

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CBoss_Ai);
    CBoss_Ai();
    ~CBoss_Ai();
};