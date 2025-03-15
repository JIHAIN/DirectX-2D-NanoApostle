#pragma once
#include "EditorUI.h"

class CFlipbook;
class CTexture;
class CGameObjectEx;
class CEditorCamScript;
class CSprite;

// ���� ���ӻ󿡼� �ε��ϴ� �� ó�� ���̵��� ������ �� ���ְ� ����� ��׶��� ����� �۵��ϰ� 
// ��Ʋ�� �ڸ��� Ȯ���� ����� ����Ű�� �� ������ �۵������ϰ� �����
// ������ ���̺� �ε�� ���� ���ѵα� �� �ڵ带 �ű�� ���� �ؼ�
// ����Ʈ ��� �ʿ��� �ε��� �� �Ѵ��� Ȯ���ϱ� ���� ����� ex �� �ε����� ���ؼ� �� �� �ְ� ����ų�
// �ִϸ��̼� ������� ��� ������ ���� �ε����� ���°���� �����ִ� â�� �����
// ��ü���� �ڵ尡 �������ϴϱ� �ѹ� �ٵ�� �͵� �ʿ��ҵ�
// ��� ������� ��� �ɰ� ���� ������ ���� Ž���� â���� �� �� �� �����ϱ� 

struct AniInfo
{
    Ptr<CTexture> Texture;
    Vec2          Texture_Resolution;
    Vec2          LeftTop;
    Vec2          Slice;
    Vec2          Offset;
    Vec2          BackGround;
};

struct AtlasInfo
{
    Ptr<CTexture> AtlasTexture;
    Vec2          Texture_Resolution;
    Vec2          LeftTop;
    Vec2          Slice;
    Vec2          Offset;
    Vec2          BackGround;
};

class AnimationUI :
    public EditorUI
{
private:
    Ptr<CFlipbook>  m_Flipbook;
    Ptr<CTexture>   m_Texture;
    AniInfo         m_AniInfo;
    AtlasInfo       m_AtlasInfo;

    //����Ʈ ����
    vector<Ptr<CSprite>> m_CurSprite;

    CGameObject*    m_PreviewObj;

    // ũ������Ƽ�� ��� ������
    string          m_TextureKey;
    string          m_TexturePath;
    string          m_FlibookKey;
    vector<char>    m_FlibookKeyBuffer;
    int             m_Idex; //�������̺z �ε��� ����� Ű���� �̸����� �̰� ����
    int             m_PrevIdex;
    string          m_SpriteKey;
    vector<char>    m_spriteKeyBuffer;

    Vec2            m_LeftTop;
    Vec2            m_Slice;
    Vec2            m_Slicefirst; // �����¿� ù ������ �����̽���
    Vec2            m_BackGround;
    Vec2            m_Offset;

    ImVec2          m_DbugTap;      // ȭ�� �����ӿ� ��
    ImVec2          m_DbugDown;     // ��������
    ImVec2          m_DbugDiff;

    ImVec2          m_PanOffset;     // �д� ������ �����
    bool            m_IsPanning;     // �д� ������ Ȯ�ο�
    ImVec2          m_LastMousePos;  // ���� ���콺 ��ġ ����

    set<Vec2>       m_PixelID;       // ��ϵ����� �ִ� �ȼ����� Ȯ���ϴ� �뵵
    ImVec2          ImageRectMin;    // Image ���� �»�� ���� ��ġ
    ImVec2          ImageRectMax;
    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;         // ���� �ػ� ��� ��� �̹����� ����

    ImVec2          m_MousePos;      // ���� ���콺 ��ġ
    ImVec2          m_MouseLT;       // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2          m_MouseRB;       // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ

    bool CreatingMode;

    // �ִϸ��̼� �����
    int m_SpriteIdx;
    float m_FPS;
    float m_Time;
    bool  m_Finish;

public:
    void ComponentTitle(const string& _title);
  
    void CreateMode();
    void EditMode();
   
    void Clear();

    Vec2 UVToPixel(int textureWidth, int textureHeight, const Vec2& uv);
    Vec2 PixelToUV(int textureWidth, int textureHeight, const Vec2& pixel);

    int SaveFlipbook(Ptr<CTexture> _Texture);
    string LoadFlipbook();
    string SelectAtlas();

    void RenderAtlasPreview();
    void RenderSpritePreview();

    void RenderAnimationPreview();
    void RenderSpritePreview2();

    void RenderDrawLine();

    void WheelCheck();

    void SelectCheck();

    void CalcSpriteSize(Vec2 _PixelPos);
    bool IsPixelOk(Vec2 _PixelPos);
public:
    virtual void Render_Update() override;

private:

public:
    AnimationUI();
    ~AnimationUI();
};