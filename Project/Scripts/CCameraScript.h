#pragma once
#include <Engine/CScript.h>

class CCameraScript :
    public CScript
{
private:
    float        m_CameraSpeed;
    CGameObject* m_Player;         // 기본 타겟(플레이어)
    CGameObject* m_CurrentTarget;  // 현재 타겟
    CGameObject* m_NextTarget;     // 전환될 타겟

    float m_TargetDuration;        // 타겟을 바라볼 시간
    float m_TransitionSpeed;       // 타겟 전환 속도
    float m_CurrentTime;           // 현재 타겟을 바라본 시간
    float m_TransitionProgress;    // 전환 진행도 (0~1)
    bool  m_IsTransitioning;       // 전환 중인지 여부



public:
    void SetTemporaryTarget(CGameObject* _target, float _duration, float _transitionSpeed);


    virtual void Tick() override;
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

private:
    void CameraOrthgraphicMove();
    void CameraPerspectiveMove();

    Vec3 Lerp(const Vec3& start, const Vec3& end, float t)
    {
        return start + (end - start) * t;
    }

public:
    CLONE(CCameraScript);
    CCameraScript();
    ~CCameraScript();
};

