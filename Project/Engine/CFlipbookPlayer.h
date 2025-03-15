#pragma once
#include "CComponent.h"

#include "CFlipbook.h"

class CFlipbookPlayer :
    public CComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;      // 재생 목록 Flipbook
    Ptr<CFlipbook>          m_CurFlipbook;      // 재생중인 Flipbook
    Ptr<CFlipbook>          m_PrevFlipbook;     // 전에 재생중이던 플립북
    int                     m_SpriteIdx;        // 재생중인 Sprite 인덱스
    bool                    m_Repeat;           // 반복모드
    float                   m_FPS;              // 재생속도
    float                   m_Time;             // 누적 시간
    float                   m_PasueTime;        // 일시정지할 시간값
    bool                    m_Finish;           // 재생 회차가 끝났을 때

    bool                    Is_TermTime;    // 텀 시간 쓸건지
    float                   m_TermTime;     // 얼만큼 줄건지
    int                     Term_Idx;       // 멈출 인덱스 선택
public:
    void AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat)
    {
        m_PasueTime = 0.f;

        // 같은거 요청하면 그냥 넘겨버리는 임시코드
        if (m_CurFlipbook == m_vecFlipbook[_Idx])
            return;
     
        m_CurFlipbook = m_vecFlipbook[_Idx];
        m_FPS = _FPS;
        m_Repeat = _Repeat;
        m_Time = 0.f;
        m_SpriteIdx = 0;
    }

    float GetAnimationProgress(); // 애니메이션 진행정도
    void SetTerm(int _TermIDX, float _TermTime) { Term_Idx = _TermIDX;  m_TermTime = _TermTime; Is_TermTime = true; }

    void Stop() { m_CurFlipbook = nullptr; }
    void Pause() { m_PasueTime = 300.f; }

    Ptr<CFlipbook> GetCurrentFlipbook() { return m_CurFlipbook; }
    Ptr<CSprite> GetCurrentSprite() { return m_CurFlipbook->GetSprite(m_SpriteIdx); }
    int GetCurIdx() { return m_SpriteIdx; }
    
    bool isFinish() { return m_Finish; }

    void Binding();
    void Clear();

public:
    virtual void FinalTick() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CFlipbookPlayer);
    CFlipbookPlayer();
    ~CFlipbookPlayer();
};

