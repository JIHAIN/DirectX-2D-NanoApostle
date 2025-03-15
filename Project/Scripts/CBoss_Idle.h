#pragma once
#include <Engine/CScript.h>

class CBoss_Idle :
    public CScript
{
private:
    float m_Time;
    bool  m_first;

public:
    virtual void Tick() {}
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void AI_Tick() override;
    virtual void AI_In() override;
    virtual void AI_Exit() override;

public:
    CLONE(CBoss_Idle);
    CBoss_Idle();
    ~CBoss_Idle();
};


// Ŭ�ε尡 ¥�� ���� �ڵ�
//class CBoss_Idle : public CScript
//{
//private:
//    // ������ ���� ������ ��� ����ü
//    struct PatternInfo
//    {
//        SCRIPT_TYPE Type;        // ������ Ÿ��
//        float Weight;            // ������ ����ġ (�������� �� ���� ���õ�)
//        bool Available;          // ���� ��� ���� ����
//        float MinDistance;       // ���� ��� ������ �ּ� �Ÿ�
//        float MaxDistance;       // ���� ��� ������ �ִ� �Ÿ�
//        vector<SCRIPT_TYPE> ComboPatterns;  // �� ���� ������ ��� ������ ���ϵ��� ���
//    };
//
//    vector<PatternInfo> m_PatternPool;      // ��� ������ ��� ���ϵ��� �����ϴ� ����
//    float m_Time;                           // ���� ��ȯ Ÿ�̸�
//    SCRIPT_TYPE m_LastPattern;              // ���������� ����� ����
//
//    // C++11 ���� ���� ���� ������
//    std::random_device m_RandomDevice;      // �ϵ���� ��� ���� ������
//    std::mt19937 m_RandomEngine;            // �޸��� Ʈ������ ���� ���� �˰���
//    std::uniform_real_distribution<float> m_Distribution;  // 0~1 ������ ���� ����
//
//public:
//    // ������: �⺻�� �ʱ�ȭ �� ���� ������ ����
//    CBoss_Idle()
//        : CScript((UINT)SCRIPT_TYPE::BOSS_IDLE)
//        , m_Time(0.f)
//        , m_LastPattern(SCRIPT_TYPE::BOSS_IDLE)
//        , m_RandomEngine(m_RandomDevice())
//        , m_Distribution(0.f, 1.f)
//    {
//        InitializePatterns();
//    }
//
//    // ��� ������ �⺻ ������ �ʱ�ȭ�ϴ� �Լ�
//    void InitializePatterns()
//    {
//        // �⺻ ���� ���� ����
//        PatternInfo norAtk;
//        norAtk.Type = SCRIPT_TYPE::BOSS_NOR_ATK1;
//        norAtk.Weight = 0.3f;              // 30% ����ġ
//        norAtk.Available = true;
//        norAtk.MinDistance = 0.f;          // ����� �Ÿ����� ��� ����
//        norAtk.MaxDistance = 300.f;
//        norAtk.ComboPatterns = { SCRIPT_TYPE::BOSS_DASH, SCRIPT_TYPE::BOSS_BOTTOM_ATK };
//        m_PatternPool.push_back(norAtk);
//
//        // �ٴ� ���� ���� ����
//        PatternInfo bottomAtk;
//        bottomAtk.Type = SCRIPT_TYPE::BOSS_BOTTOM_ATK;
//        bottomAtk.Weight = 0.2f;           // 20% ����ġ
//        bottomAtk.Available = true;
//        bottomAtk.MinDistance = 100.f;     // �߰Ÿ����� ���
//        bottomAtk.MaxDistance = 400.f;
//        bottomAtk.ComboPatterns = { SCRIPT_TYPE::BOSS_JUMP_ATK };
//        m_PatternPool.push_back(bottomAtk);
//
//        // ������ ���ϵ鵵 ����� ������� ����...
//        // [���� ���� �ʱ�ȭ �ڵ�� ����]
//    }
//
//    // ������ �÷��̾� ������ �Ÿ��� ����ϴ� �Լ�
//    float GetDistanceToPlayer()
//    {
//        // ���� AI ������Ʈ ��������
//        CBoss_Ai* bossAI = (CBoss_Ai*)GetOwner()->GetScript((UINT)SCRIPT_TYPE::BOSS_AI);
//        if (!bossAI || !bossAI->GetTargetPlayer())
//            return 0.f;
//
//        // ������ �÷��̾��� ��ġ ���̷� �Ÿ� ���
//        Vec3 playerPos = bossAI->GetTargetPlayer()->Transform()->GetWorldPos();
//        Vec3 bossPos = Transform()->GetWorldPos();
//        return (playerPos - bossPos).Length();
//    }
//
//    // �־��� ������ ���� ��Ȳ���� ��� �������� �˻��ϴ� �Լ�
//    bool IsPatternAvailable(const PatternInfo& pattern, float distance)
//    {
//        // �Ÿ� ���� ���� �˻�
//        if (distance < pattern.MinDistance || distance > pattern.MaxDistance)
//            return false;
//
//        // �޺� ���� ���� �˻� (���� ������ �ִ� ���)
//        if (m_LastPattern != SCRIPT_TYPE::BOSS_IDLE)
//        {
//            // ���� ������ ���� ã��
//            auto it = std::find_if(m_PatternPool.begin(), m_PatternPool.end(),
//                [this](const PatternInfo& p) { return p.Type == m_LastPattern; });
//
//            if (it != m_PatternPool.end())
//            {
//                // ���� ������ ���� ������ �޺� ��Ͽ� �ִ��� Ȯ��
//                auto& combos = it->ComboPatterns;
//                if (std::find(combos.begin(), combos.end(), pattern.Type) == combos.end())
//                    return false;
//            }
//        }
//
//        return true;
//    }
//
//    // ���� ���� ������ �����ϴ� �Լ�
//    SCRIPT_TYPE SelectNextPattern()
//    {
//        float distance = GetDistanceToPlayer();
//        vector<pair<SCRIPT_TYPE, float>> availablePatterns;  // ��� ������ ���ϵ� ����
//        float totalWeight = 0.f;  // ��ü ����ġ ��
//
//        // ���� ��Ȳ���� ��� ������ ���ϵ� ���͸�
//        for (const auto& pattern : m_PatternPool)
//        {
//            if (IsPatternAvailable(pattern, distance))
//            {
//                availablePatterns.push_back({ pattern.Type, pattern.Weight });
//                totalWeight += pattern.Weight;
//            }
//        }
//
//        // ��� ������ ������ ������ �⺻ ���� ��ȯ
//        if (availablePatterns.empty())
//            return SCRIPT_TYPE::BOSS_NOR_ATK1;
//
//        // ����ġ ��� ���� ����
//        float randomValue = m_Distribution(m_RandomEngine);
//        float currentWeight = 0.f;
//
//        for (const auto& pattern : availablePatterns)
//        {
//            currentWeight += pattern.second / totalWeight;
//            if (randomValue <= currentWeight)
//            {
//                m_LastPattern = pattern.first;
//                return pattern.first;
//            }
//        }
//
//        return availablePatterns[0].first;  // ������ġ
//    }
//
//    // �� �����Ӹ��� ȣ��Ǵ� ������Ʈ �Լ�
//    void AI_Tick()
//    {
//        m_Time += DT;  // ��� �ð� ����
//
//        // ���� �ð��� ������ ���ο� ���� ����
//        if (m_Time > 1.f)
//        {
//            SCRIPT_TYPE nextPattern = SelectNextPattern();
//            B_CHANGE(nextPattern);  // ���� ��ȯ
//            m_Time = 0.f;  // Ÿ�̸� ����
//        }
//    }
//};