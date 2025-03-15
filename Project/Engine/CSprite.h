#pragma once
#include "CAsset.h"

#include "CTexture.h"

class CSprite :
    public CAsset
{
private:
    Ptr<CTexture>       m_Atlas;
    Vec2                m_LeftTopUV;
    Vec2                m_SliceUV;

    Vec2                m_BackgroundUV;
    Vec2                m_OffsetUV;

public:
    void SetAtlasTexture(Ptr<CTexture> _Tex) { m_Atlas = _Tex; }
    void SetLeftTop(Vec2 _LeftTopPixel);
    void SetSlice(Vec2 _SlicePixel);
    void SetBackground(Vec2 _BackgroundPixel);
    void SetOffset(Vec2 _OffsetPixel);

    Ptr<CTexture> GetAtlasTexture() { return m_Atlas; }
    Vec2 GetLeftTop() { return m_LeftTopUV; }
    Vec2 GetSlice() { return m_SliceUV; }
    Vec2 GetBackground() { return m_BackgroundUV; }
    Vec2 GetOffset() { return m_OffsetUV; }

    Vec2 GetPixelLeftTop()    { return Vec2(m_LeftTopUV.x * m_Atlas->GetWidth(), m_LeftTopUV.y * m_Atlas->GetHeight()); }
    Vec2 GetPixelSlice()      { return Vec2(m_SliceUV.x * m_Atlas->GetWidth(), m_SliceUV.y * m_Atlas->GetHeight());}
    Vec2 GetPixelBackground() { return Vec2(m_BackgroundUV.x * m_Atlas->GetWidth(), m_BackgroundUV.y * m_Atlas->GetHeight());}
    Vec2 GetPixelOffset()     { return Vec2(m_OffsetUV.x * m_Atlas->GetWidth(), m_OffsetUV.y * m_Atlas->GetHeight());}

    void Binding();
    static void Clear();

private:
    virtual int Load(const wstring& _FilePath) override;

public:
    virtual int Save(const wstring& _strFilePath) override;

public:
    CLONE_DISABLE(CSprite);
    CSprite(bool _bEngineRes = false);
    ~CSprite();
};
