#pragma once
#include "ComponentUI.h"

class CTexture;

class ParticleUI :
    public ComponentUI
{
private:
    Ptr<CTexture>   m_Texture;
    string          m_TextureKey;
    string          m_TexturePath;

public:
    virtual void Render_Update() override;
    void CheakBoxList(tParticleModule* _Module);

    void DrawLabeledInput(const char* label, const std::function<void()>& inputWidget);
    string SetParticleTexture();

    int SaveParticle(Ptr<CTexture> _Texture);

public:
    ParticleUI();
    ~ParticleUI();
};

