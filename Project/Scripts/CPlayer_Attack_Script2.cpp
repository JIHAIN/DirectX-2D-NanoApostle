#include "pch.h"
#include "CPlayer_Attack_Script2.h"
#include "CPlayer_Ai.h"
#include "CPlayerScript.h"
#include "CPlayer_Atk_Obj.h"

CPlayer_Attack_Script2::CPlayer_Attack_Script2()
    : CScript((UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT2)
    , m_AtkDmg(0)
    , m_AtkRange(20)
    , m_AtkTime(0.f)
    , MousDir(0.f, 0.f, 0.f)
{
    SetAI(true);
}

CPlayer_Attack_Script2::~CPlayer_Attack_Script2()
{
}

void CPlayer_Attack_Script2::Begin()
{
    SetAI(true);

}

void CPlayer_Attack_Script2::AI_In()
{
    Vec3 MousePos = g_MouseInfo.MouseWorldPos;
    Vec3 pWPos = Transform()->GetWorldPos();
    Vec3 PlayerMouseDir = MousePos - pWPos;
    MousDir = PlayerMouseDir.Normalize();

    CreateAtk2();
    PlayerAniPlay2(MousDir);
    RigidBody()->AddForce(Vec2(MousDir.x * 300.f, MousDir.y * 300.f));

    m_AtkTime = 0.f;

    // Sound 로딩하기
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\PlayerTentacleSlash.wav", L"Sound\\PlayerTentacleSlash.wav");
    pSound->Play(1, 0.7f, true);
}

void CPlayer_Attack_Script2::AI_Tick()
{
    m_AtkTime += DT;

    if (KEY_TAP(KEY::SPACE))
    {
        P_CHANGE(SCRIPT_TYPE::PLAYER_DASH);
        return;
    }

    if (KEY_TAP(KEY::RBTN))
    {
        P_CHANGE(SCRIPT_TYPE::SHIELD_SCRIPT);
        return;
    }

    // 1.7초에서 2.8초 사이에 공격 입력이 들어오면 다음 공격으로
    if (KEY_TAP(KEY::LBTN) && m_AtkTime >= 0.2f && m_AtkTime <= 1.3f)
    {
        P_CHANGE(SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT3);
        return;
    }

    // 애니메이션이 끝나거나 시간이 초과되면 기본 상태로
    if (FlipbookPlayer()->isFinish() || m_AtkTime > 1.3f)
    {
        P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
        return;
    }
}

void CPlayer_Attack_Script2::AI_Exit()
{
    m_AtkTime = 0.f;
}

void CPlayer_Attack_Script2::CreateAtk2()
{
    Vec3 pPos = Transform()->GetRelativePos();
    pPos += MousDir * m_AtkRange;

    float angle = atan2(MousDir.y, MousDir.x);
    float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;


    float baseScaleX = 200.0f;
    float baseScaleY = 200.0f;

    CGameObject* PlayerAtk2 = new CGameObject;
    PlayerAtk2->AddComponent(new CMeshRender);
    PlayerAtk2->AddComponent(new CCollider2D);
    PlayerAtk2->AddComponent(new CFlipbookPlayer);
    PlayerAtk2->AddComponent(new CPlayer_Atk_Obj);
    PlayerAtk2->SetName(L"pAtk2");

    PlayerAtk2->AddComponent(new CLight2D);
    PlayerAtk2->Light2D()->SetLightColor(Vec3(0.1f, 0.1f, 0.1f));
    PlayerAtk2->Light2D()->SetLightType(LIGHT_TYPE::POINT);
    PlayerAtk2->Light2D()->SetRadius(50.f);

    PlayerAtk2->Collider2D()->SetIndependentScale(false);
    PlayerAtk2->Collider2D()->SetScale(Vec2(0.7f, 0.7f));
             
    PlayerAtk2->Transform()->SetRelativePos(pPos);
    PlayerAtk2->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
    PlayerAtk2->Transform()->SetIndependentScale(true);
    PlayerAtk2->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, correctedAngle));

    PlayerAtk2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
    PlayerAtk2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));

    PlayerAtk2->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
    PlayerAtk2->FlipbookPlayer()->Play(0, 130.f, false);

    CreateObject(PlayerAtk2, 4, false);
}

void CPlayer_Attack_Script2::PlayerAniPlay2(Vec3 _Dir)
{
    float angle = atan2(_Dir.x, _Dir.y) * 180.0f / XM_PI;

    if (angle >= -45 && angle < 45)  // 위쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Up2, 10.f, false);
    else if (angle >= 45 && angle < 135)  // 오른쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right2, 10.f, false);
    else if (angle >= -135 && angle < -45)  // 왼쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right2, 10.f, false);
    else  // 아래쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Down2, 10.f, false);
}

void CPlayer_Attack_Script2::Tick() {}
void CPlayer_Attack_Script2::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
void CPlayer_Attack_Script2::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
void CPlayer_Attack_Script2::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}

void CPlayer_Attack_Script2::SaveComponent(FILE* _File)
{
    fwrite(&m_AtkDmg, sizeof(int), 1, _File);
    fwrite(&m_AtkRange, sizeof(float), 1, _File);
}

void CPlayer_Attack_Script2::LoadComponent(FILE* _File)
{
    fread(&m_AtkDmg, sizeof(int), 1, _File);
    fread(&m_AtkRange, sizeof(float), 1, _File);

    m_AtkTime = 0.f;
    MousDir = Vec3(0.f, 0.f, 0.f);
}