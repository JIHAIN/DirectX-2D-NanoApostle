#include "pch.h"
#include "SpriteUI.h"

SpriteUI::SpriteUI()
	: AssetUI("Sprite", ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Render_Update()
{
	AssetTitle();

    Ptr<CSprite> pAsset = dynamic_cast<CSprite*>(GetAsset().Get());
    assert(pAsset.Get());

    ImGui::Text("Name");
    ImGui::SameLine(100);

    string strKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

    pAsset->GetKey();
   
    pAsset->GetBackground();
    pAsset->GetOffset();

    ImGui::InputText("##MeshName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

    Ptr<CTexture> pTex = pAsset->GetAtlasTexture();

    int width = pTex->GetWidth();
    int height = pTex->GetHeight();

    // UV ÁÂÇ¥¸¦ ÇÈ¼¿ ÁÂÇ¥·Î º¯È¯ÇØ¼­ ÀúÀå½ÃÅ´
    Vec2 LeftTop = UVToPixel(width, height, pAsset->GetLeftTop());
    Vec2 Slice = UVToPixel(width, height, pAsset->GetSlice());
    Vec2 BackGround = UVToPixel(width, height, pAsset->GetBackground());
    Vec2 Offset = UVToPixel(width, height, pAsset->GetOffset());

    float Lx = pAsset->GetLeftTop().x;
    float Ly = pAsset->GetLeftTop().y;
    float Sx = pAsset->GetSlice().x;
    float Sy = pAsset->GetSlice().y;

    float a = 70.f;

    ImGui::Text("LeftTop");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##LeftTopx", &LeftTop.x, 0.f, 0.f);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##LeftTopy", &LeftTop.y, 0.f, 0.f);

    ImGui::Text("Slice");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##Slicex", &Slice.x, 0.f, 0.f);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##Slicey", &Slice.y, 0.f, 0.f);

    ImGui::Text("BackGround");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##BackGroundx", &BackGround.x, 0.f, 0.f);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##BackGroundy", &BackGround.y, 0.f, 0.f);

    ImGui::Text("Offset");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##Offsetx", &Offset.x, 0.f, 0.f);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(a);
    ImGui::InputFloat("##Offsety", &Offset.y, 0.f, 0.f);

    ImGui::Image(pTex->GetSRV().Get(),
        ImVec2(300.f, 300.f),
        ImVec2(Lx, Ly),
        ImVec2(Lx + Sx, Ly + Sy));
}

Vec2 SpriteUI::UVToPixel(int textureWidth, int textureHeight, const Vec2& uv)
{
   return Vec2(uv.x * textureWidth, uv.y * textureHeight);
}
