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


// 클로드가 짜준 패턴 코드
//class CBoss_Idle : public CScript
//{
//private:
//    // 패턴의 세부 정보를 담는 구조체
//    struct PatternInfo
//    {
//        SCRIPT_TYPE Type;        // 패턴의 타입
//        float Weight;            // 패턴의 가중치 (높을수록 더 자주 선택됨)
//        bool Available;          // 패턴 사용 가능 여부
//        float MinDistance;       // 패턴 사용 가능한 최소 거리
//        float MaxDistance;       // 패턴 사용 가능한 최대 거리
//        vector<SCRIPT_TYPE> ComboPatterns;  // 이 패턴 다음에 사용 가능한 패턴들의 목록
//    };
//
//    vector<PatternInfo> m_PatternPool;      // 사용 가능한 모든 패턴들을 저장하는 벡터
//    float m_Time;                           // 패턴 전환 타이머
//    SCRIPT_TYPE m_LastPattern;              // 마지막으로 사용한 패턴
//
//    // C++11 난수 생성 관련 변수들
//    std::random_device m_RandomDevice;      // 하드웨어 기반 난수 생성기
//    std::mt19937 m_RandomEngine;            // 메르센 트위스터 난수 생성 알고리즘
//    std::uniform_real_distribution<float> m_Distribution;  // 0~1 사이의 균일 분포
//
//public:
//    // 생성자: 기본값 초기화 및 난수 생성기 설정
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
//    // 모든 패턴의 기본 정보를 초기화하는 함수
//    void InitializePatterns()
//    {
//        // 기본 공격 패턴 설정
//        PatternInfo norAtk;
//        norAtk.Type = SCRIPT_TYPE::BOSS_NOR_ATK1;
//        norAtk.Weight = 0.3f;              // 30% 가중치
//        norAtk.Available = true;
//        norAtk.MinDistance = 0.f;          // 가까운 거리에서 사용 가능
//        norAtk.MaxDistance = 300.f;
//        norAtk.ComboPatterns = { SCRIPT_TYPE::BOSS_DASH, SCRIPT_TYPE::BOSS_BOTTOM_ATK };
//        m_PatternPool.push_back(norAtk);
//
//        // 바닥 공격 패턴 설정
//        PatternInfo bottomAtk;
//        bottomAtk.Type = SCRIPT_TYPE::BOSS_BOTTOM_ATK;
//        bottomAtk.Weight = 0.2f;           // 20% 가중치
//        bottomAtk.Available = true;
//        bottomAtk.MinDistance = 100.f;     // 중거리에서 사용
//        bottomAtk.MaxDistance = 400.f;
//        bottomAtk.ComboPatterns = { SCRIPT_TYPE::BOSS_JUMP_ATK };
//        m_PatternPool.push_back(bottomAtk);
//
//        // 나머지 패턴들도 비슷한 방식으로 설정...
//        // [이하 패턴 초기화 코드는 동일]
//    }
//
//    // 보스와 플레이어 사이의 거리를 계산하는 함수
//    float GetDistanceToPlayer()
//    {
//        // 보스 AI 컴포넌트 가져오기
//        CBoss_Ai* bossAI = (CBoss_Ai*)GetOwner()->GetScript((UINT)SCRIPT_TYPE::BOSS_AI);
//        if (!bossAI || !bossAI->GetTargetPlayer())
//            return 0.f;
//
//        // 보스와 플레이어의 위치 차이로 거리 계산
//        Vec3 playerPos = bossAI->GetTargetPlayer()->Transform()->GetWorldPos();
//        Vec3 bossPos = Transform()->GetWorldPos();
//        return (playerPos - bossPos).Length();
//    }
//
//    // 주어진 패턴이 현재 상황에서 사용 가능한지 검사하는 함수
//    bool IsPatternAvailable(const PatternInfo& pattern, float distance)
//    {
//        // 거리 제한 조건 검사
//        if (distance < pattern.MinDistance || distance > pattern.MaxDistance)
//            return false;
//
//        // 콤보 패턴 조건 검사 (이전 패턴이 있는 경우)
//        if (m_LastPattern != SCRIPT_TYPE::BOSS_IDLE)
//        {
//            // 이전 패턴의 정보 찾기
//            auto it = std::find_if(m_PatternPool.begin(), m_PatternPool.end(),
//                [this](const PatternInfo& p) { return p.Type == m_LastPattern; });
//
//            if (it != m_PatternPool.end())
//            {
//                // 현재 패턴이 이전 패턴의 콤보 목록에 있는지 확인
//                auto& combos = it->ComboPatterns;
//                if (std::find(combos.begin(), combos.end(), pattern.Type) == combos.end())
//                    return false;
//            }
//        }
//
//        return true;
//    }
//
//    // 다음 공격 패턴을 선택하는 함수
//    SCRIPT_TYPE SelectNextPattern()
//    {
//        float distance = GetDistanceToPlayer();
//        vector<pair<SCRIPT_TYPE, float>> availablePatterns;  // 사용 가능한 패턴들 저장
//        float totalWeight = 0.f;  // 전체 가중치 합
//
//        // 현재 상황에서 사용 가능한 패턴들 필터링
//        for (const auto& pattern : m_PatternPool)
//        {
//            if (IsPatternAvailable(pattern, distance))
//            {
//                availablePatterns.push_back({ pattern.Type, pattern.Weight });
//                totalWeight += pattern.Weight;
//            }
//        }
//
//        // 사용 가능한 패턴이 없으면 기본 공격 반환
//        if (availablePatterns.empty())
//            return SCRIPT_TYPE::BOSS_NOR_ATK1;
//
//        // 가중치 기반 랜덤 선택
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
//        return availablePatterns[0].first;  // 안전장치
//    }
//
//    // 매 프레임마다 호출되는 업데이트 함수
//    void AI_Tick()
//    {
//        m_Time += DT;  // 경과 시간 누적
//
//        // 일정 시간이 지나면 새로운 패턴 선택
//        if (m_Time > 1.f)
//        {
//            SCRIPT_TYPE nextPattern = SelectNextPattern();
//            B_CHANGE(nextPattern);  // 패턴 전환
//            m_Time = 0.f;  // 타이머 리셋
//        }
//    }
//};