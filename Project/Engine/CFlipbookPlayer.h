#pragma once
#include "CComponent.h"

#include "CFlipbook.h"

class CFlipbookPlayer :
    public CComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;      // ��� ��� Flipbook
    Ptr<CFlipbook>          m_CurFlipbook;      // ������� Flipbook
    Ptr<CFlipbook>          m_PrevFlipbook;     // ���� ������̴� �ø���
    int                     m_SpriteIdx;        // ������� Sprite �ε���
    bool                    m_Repeat;           // �ݺ����
    float                   m_FPS;              // ����ӵ�
    float                   m_Time;             // ���� �ð�
    float                   m_PasueTime;        // �Ͻ������� �ð���
    bool                    m_Finish;           // ��� ȸ���� ������ ��

    bool                    Is_TermTime;    // �� �ð� ������
    float                   m_TermTime;     // ��ŭ �ٰ���
    int                     Term_Idx;       // ���� �ε��� ����
public:
    void AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat)
    {
        m_PasueTime = 0.f;

        // ������ ��û�ϸ� �׳� �Ѱܹ����� �ӽ��ڵ�
        if (m_CurFlipbook == m_vecFlipbook[_Idx])
            return;
     
        m_CurFlipbook = m_vecFlipbook[_Idx];
        m_FPS = _FPS;
        m_Repeat = _Repeat;
        m_Time = 0.f;
        m_SpriteIdx = 0;
    }

    float GetAnimationProgress(); // �ִϸ��̼� ��������
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

