#pragma once
#include <Engine/CScript.h>

class CPlayer_Ai :
    public CScript
{
private:
    CScript*           m_CurScript; // 현재 Ai Tick을 줄 스크립트

    CPlayer_Dash_State m_state;     // 플레이어 상태정보
    CGameObject*       HitEffect;


    Vec3               m_PlayerDir; // 플레이어가 가야할 방향
    Vec3               m_PlayerPrevPos; // 이전 틱 플레이어 위치
    bool               m_HitOn;
    bool               m_Invincible; //무적 상태

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
    void  ActivateInvincible() { m_Invincible = true; } // 무적 만들기
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

