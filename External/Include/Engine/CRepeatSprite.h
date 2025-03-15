// CRepeatSprite.h
#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"

class CRepeatSprite :
    public CRenderComponent
{
private:
    Vec2            m_RepeatCount;     // x, y ������ �ݺ��� Ƚ��
    Vec2            m_SingleImageSize; // �� �̹����� ũ��
    Ptr<CTexture>   m_Texture;        // �ݺ��� �ؽ�ó

public:
    void SetRepeatCount(UINT _x, UINT _y) 
    { 
        m_RepeatCount = Vec2((float)_x, (float)_y);
        UpdateScale();
    }
    void SetSingleImageSize(float _x, float _y) 
    { 
        m_SingleImageSize = Vec2(_x, _y);
        UpdateScale();
    }
    void SetTexture(Ptr<CTexture> _Tex) { m_Texture = _Tex; }
    void UpdateScale();

 /*   void CalculateVisibleArea(Vec2& outStartRepeat, Vec2& outEndRepeat);
    bool IsInScreen(const Vec3& pos);*/

public:
    virtual void Init() override;
    virtual void FinalTick() override {};
    virtual void Render() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CRepeatSprite);
    CRepeatSprite();
    ~CRepeatSprite();
};