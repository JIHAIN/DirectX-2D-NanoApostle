#pragma once
#include "EditorUI.h"

class CFlipbook;
class CTexture;
class CGameObjectEx;
class CEditorCamScript;
class CSprite;

// 실제 게임상에서 로드하는 것 처럼 보이도록 랜더링 할 수있게 만들기 백그라운드 제대로 작동하게 
// 아틀라스 자를때 확대기능 만들기 단축키로 좀 빠르게 작동가능하게 만들기
// 선생님 세이브 로드와 연동 시켜두기 내 코드를 옮기든 뭐든 해서
// 에디트 모드 쪽에서 인덱스 별 한눈에 확인하기 쉽게 만들기 ex 전 인덱스랑 비교해서 볼 수 있게 만들거나
// 애니메이션 재생관련 배속 조절과 현재 인덱스가 몇번째인지 보여주는 창도 만들기
// 전체적인 코드가 지저분하니까 한번 다듬는 것도 필요할듯
// 모달 방식으로 열어도 될거 같음 어차피 파일 탐색기 창에서 다 볼 수 있으니까 

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

    //에디트 모드용
    vector<Ptr<CSprite>> m_CurSprite;

    CGameObject*    m_PreviewObj;

    // 크리에이티브 모드 변수들
    string          m_TextureKey;
    string          m_TexturePath;
    string          m_FlibookKey;
    vector<char>    m_FlibookKeyBuffer;
    int             m_Idex; //스프라이틑 인덱스 저장용 키값의 이름에도 이걸 넣음
    int             m_PrevIdex;
    string          m_SpriteKey;
    vector<char>    m_spriteKeyBuffer;

    Vec2            m_LeftTop;
    Vec2            m_Slice;
    Vec2            m_Slicefirst; // 오프셋용 첫 프레임 슬라이스값
    Vec2            m_BackGround;
    Vec2            m_Offset;

    ImVec2          m_DbugTap;      // 화면 움직임용 탭
    ImVec2          m_DbugDown;     // 누르는중
    ImVec2          m_DbugDiff;

    ImVec2          m_PanOffset;     // 패닝 오프셋 저장용
    bool            m_IsPanning;     // 패닝 중인지 확인용
    ImVec2          m_LastMousePos;  // 이전 마우스 위치 저장

    set<Vec2>       m_PixelID;       // 등록된적이 있는 픽셀인지 확인하는 용도
    ImVec2          ImageRectMin;    // Image 위젯 좌상단 실제 위치
    ImVec2          ImageRectMax;
    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;         // 실제 해상도 대비 출력 이미지의 비율

    ImVec2          m_MousePos;      // 실제 마우스 위치
    ImVec2          m_MouseLT;       // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2          m_MouseRB;       // 마우스 위치에 해당하는 Texture 의 픽셀 좌표

    bool CreatingMode;

    // 애니메이션 재생용
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