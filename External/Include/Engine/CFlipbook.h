#pragma once
#include "CAsset.h"
#include "CSprite.h"
#include "CTexture.h"

class CFlipbook :
    public CAsset
{
private:
    vector<Ptr<CSprite>>    m_vecSprite;
    Ptr<CTexture>           m_Atlas; // 로드할때용도

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    UINT GetMaxSprite() { return (UINT)m_vecSprite.size(); }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }

    vector<Ptr<CSprite>> GetVecSprite() { return m_vecSprite; }

public:
    virtual int Save(const wstring& _strFilePath) override;

private:
    virtual int Load(const wstring& _FilePath) override;

public:
    CLONE_DISABLE(CFlipbook);
    CFlipbook(bool _bEngineRes = false);
    ~CFlipbook();
};

