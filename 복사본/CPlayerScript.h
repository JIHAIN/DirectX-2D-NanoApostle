#pragma once
#include <Engine/CScript.h>

enum class CPlayer_Dash_State;

class CPlayerScript :
    public CScript
{
private:
    float   m_PlayerSpeed;
    float   m_PaperBurnIntence;
    Vec3    m_PlayerPrevPos;
    Vec3    m_PlayerCurPos;
    Vec3    m_PlayerMoveDir; // 이전위치에서 어디로 갔는지 
    Vec3    m_PlayerDir;
    CPlayer_Dash_State  m_state;

    bool    m_PlayDash;
    bool    Player_Move[4];

    CGameObject* pPostProcess;
    CGameObject* pPostProcess1;
    CGameObject* pPostProcess2;



public:
    void Set_Player_Move_limit(int _int, bool _limit) { Player_Move[_int] = _limit; } // 0.Left , 1.Right, 2.Up, 3.Down.
    Vec3 GetPlayerDir() { return m_PlayerDir; }
    Vec3 GetMoveDir() { return m_PlayerMoveDir; }
    void SetPlayerDir(Vec3 _Dir) { m_PlayerDir = _Dir; }
    CPlayer_Dash_State GetDashState() { return m_state; }

    bool isDash() { return m_PlayDash; }
    void SetDash(bool _PlayDash) { m_PlayDash = _PlayDash; }

    void PostProsed(); // 임시 코드

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);


public:
    CLONE_DISABLE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};