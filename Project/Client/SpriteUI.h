#pragma once
#include "AssetUI.h"
class SpriteUI :
    public AssetUI
{
private:

public:
    virtual void Render_Update() override;
    Vec2 UVToPixel(int textureWidth, int textureHeight, const Vec2& uv);
  
public:
    SpriteUI();
    ~SpriteUI();
};

