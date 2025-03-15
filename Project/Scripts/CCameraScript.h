#pragma once
#include <Engine/CScript.h>

class CCameraScript :
    public CScript
{
private:
    float        m_CameraSpeed;
    CGameObject* m_Player;         // �⺻ Ÿ��(�÷��̾�)
    CGameObject* m_CurrentTarget;  // ���� Ÿ��
    CGameObject* m_NextTarget;     // ��ȯ�� Ÿ��

    float m_TargetDuration;        // Ÿ���� �ٶ� �ð�
    float m_TransitionSpeed;       // Ÿ�� ��ȯ �ӵ�
    float m_CurrentTime;           // ���� Ÿ���� �ٶ� �ð�
    float m_TransitionProgress;    // ��ȯ ���൵ (0~1)
    bool  m_IsTransitioning;       // ��ȯ ������ ����



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

