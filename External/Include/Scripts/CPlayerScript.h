#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    float            m_PlayerSpeed;
    float            m_DustTime;

    bool             Player_Move[4];

    Player_Ani_State m_CurAnimState;  // 현재 애니메이션 상태 추가
    CGameObject*     pPostProcess;
    CGameObject*     pPostProcess1;
    CGameObject*     pPostProcess2;


public:
    void Set_Player_Move_limit(int _int, bool _limit) { Player_Move[_int] = _limit; } // 0.Left , 1.Right, 2.Up, 3.Down.
    const bool(&Get_Player_Move_limit())[4] { return Player_Move; }   

    void CreateDust();
    void PostProsed(); // 임시 코드

    virtual void Begin() override;
    virtual void Tick() {};
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE_DISABLE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};