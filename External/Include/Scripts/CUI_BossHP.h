#pragma once
#include <Engine/CScript.h>

class CUI_BossHP :
    public CScript
{
private:
    float        m_MaxHP;
    float        m_CurrentHP;
    float        m_OriginalScalex;
    CGameObject* m_TargetObject;  // HP를 표시할 대상 오브젝트
    Vec3         m_OriginalPos;  // 초기 위치 저장
    bool         m_Death;

public:
    void SetTarget(CGameObject* _Target) { m_TargetObject = _Target; }
    void HitHP(float _CurrentHP) { m_CurrentHP -= _CurrentHP; }
    bool IsDeath() { return m_Death; }
public:

    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CUI_BossHP);
    CUI_BossHP();
    ~CUI_BossHP();
};