#pragma once
#include <Engine/CScript.h>

class CPlayer_Ai :
    public CScript
{
private:
    CScript*           m_CurScript; // ���� Ai Tick�� �� ��ũ��Ʈ

    CPlayer_Dash_State m_state;     // �÷��̾� ��������
    CGameObject*       HitEffect;


    Vec3               m_PlayerDir; // �÷��̾ ������ ����
    Vec3               m_PlayerPrevPos; // ���� ƽ �÷��̾� ��ġ
    bool               m_HitOn;
    bool               m_Invincible; //���� ����

    float              m_InvinTime;
    
    CGameObject*       m_GroggyMonster;
public:
    virtual void Tick() override;
    virtual void Begin() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    void PlayerLeft();
    void PlayerDir();

    bool IsDeathState();

    CPlayer_Dash_State Get_Player_Dash_State() { return m_state; }
    Vec3  Get_Player_Dir() { return m_PlayerDir; }
    Vec3  Get_Player_PrevPos() { return m_PlayerPrevPos; }
    void  ActivateInvincible() { m_Invincible = true; } // ���� �����
    void Set_Groggy_Monster(CGameObject* _Monster);
    CGameObject* GetGroggyMonster() { return m_GroggyMonster; }

    void ChangeAi(SCRIPT_TYPE _Type);
    void Ai_PlayerAni(Player_Ani_State _Type, float _Time, bool _Re);

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CPlayer_Ai);
    CPlayer_Ai();
    ~CPlayer_Ai();
};

