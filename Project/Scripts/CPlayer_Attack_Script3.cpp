#include "pch.h"
#include "CPlayer_Attack_Script3.h"
#include "CPlayer_Ai.h"
#include "CPlayer_Atk_Obj.h"

CPlayer_Attack_Script3::CPlayer_Attack_Script3()
    : CScript((UINT)SCRIPT_TYPE::PLAYER_ATTACK_SCRIPT3)
    , m_AtkDmg(0)
    , m_AtkRange(20)
    , MousDir(0.f, 0.f, 0.f)
{
    SetAI(true);
}

CPlayer_Attack_Script3::~CPlayer_Attack_Script3()
{
}

void CPlayer_Attack_Script3::Begin()
{
    SetAI(true);

}

void CPlayer_Attack_Script3::AI_In()
{
    Vec3 MousePos = g_MouseInfo.MouseWorldPos;
    Vec3 pWPos = Transform()->GetWorldPos();
    Vec3 PlayerMouseDir = MousePos - pWPos;
    MousDir = PlayerMouseDir.Normalize();

    CreateAtk3_1();
    PlayerAniPlay3(MousDir);
    RigidBody()->AddForce(Vec2(MousDir.x * 300.f, MousDir.y * 300.f));

    // Sound 로딩하기
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\PlayerTentacleSlash.wav", L"Sound\\PlayerTentacleSlash.wav");
    pSound->Play(1, 0.7f, true);
}

void CPlayer_Attack_Script3::AI_Tick()
{
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

    // 애니메이션이 끝나면 기본 상태로
    if (FlipbookPlayer()->isFinish())
    {
        P_CHANGE(SCRIPT_TYPE::PLAYERSCRIPT);
        return;
    }
}

void CPlayer_Attack_Script3::AI_Exit()
{
}

void CPlayer_Attack_Script3::CreateAtk3_1()
{
    Vec3 pPos = Transform()->GetRelativePos();
    pPos += MousDir * m_AtkRange;

    float angle = atan2(MousDir.y, MousDir.x);
    float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

    float baseScaleX = 200.0f;
    float baseScaleY = 200.0f;

    CGameObject* PlayerAtk = new CGameObject;
    PlayerAtk->AddComponent(new CMeshRender);
    PlayerAtk->AddComponent(new CCollider2D);
    PlayerAtk->AddComponent(new CFlipbookPlayer);
    PlayerAtk->AddComponent(new CPlayer_Atk_Obj);
    PlayerAtk->SetName(L"pAtk3_1");

    PlayerAtk->AddComponent(new CLight2D);
    PlayerAtk->Light2D()->SetLightColor(Vec3(0.1f, 0.1f, 0.1f));
    PlayerAtk->Light2D()->SetLightType(LIGHT_TYPE::POINT);
    PlayerAtk->Light2D()->SetRadius(50.f);

    PlayerAtk->Collider2D()->SetIndependentScale(false);
    PlayerAtk->Collider2D()->SetScale(Vec2(0.7f, 0.7f));

    PlayerAtk->Transform()->SetRelativePos(Vec3(pPos.x, pPos.y, pPos.z));
    PlayerAtk->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
    PlayerAtk->Transform()->SetIndependentScale(true);
    PlayerAtk->Transform()->SetRelativeRotation(Vec3((XM_PI / 4), 0, correctedAngle - (XM_PI / 2.3f)));

    PlayerAtk->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    PlayerAtk->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));

    PlayerAtk->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
    PlayerAtk->FlipbookPlayer()->Play(0, 100.f, false);

    CreateObject(PlayerAtk, 4, false);

    CPlayer_Atk_Obj* PlayerAtk3 = (CPlayer_Atk_Obj*)PlayerAtk->GetScript((UINT)SCRIPT_TYPE::PLAYER_ATK_OBJ);

    PlayerAtk3->AddAtkObjDelegate(this, (SCR_DELEGATE)&CPlayer_Attack_Script3::CreateAtk3_2);
}

void CPlayer_Attack_Script3::CreateAtk3_2()
{
    Vec3 pPos = Transform()->GetRelativePos();
    pPos += MousDir * m_AtkRange;

    float angle = atan2(MousDir.y, MousDir.x);
    float correctedAngle = angle + (XM_PI / 6.0f) + 0.2f;

    float baseScaleX = 200.0f;
    float baseScaleY = 200.0f;

    CGameObject* PlayerAtk2 = new CGameObject;
    PlayerAtk2->AddComponent(new CMeshRender);
    PlayerAtk2->AddComponent(new CFlipbookPlayer);
    PlayerAtk2->AddComponent(new CPlayer_Atk_Obj);
    PlayerAtk2->SetName(L"pAtk3_2");

    PlayerAtk2->Transform()->SetRelativePos(pPos);
    PlayerAtk2->Transform()->SetRelativeScale(Vec3(baseScaleX, baseScaleY, 1.f));
    PlayerAtk2->Transform()->SetIndependentScale(true);
    PlayerAtk2->Transform()->SetRelativeRotation(Vec3((XM_PI * 1.3f), XM_PI / 5, correctedAngle - XM_PI * 1.3f));

    PlayerAtk2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
    PlayerAtk2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkillMtrl"));

    PlayerAtk2->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"PlayerAtk"));
    PlayerAtk2->FlipbookPlayer()->Play(0, 100.f, false);

    CreateObject(PlayerAtk2, 4, false);
}

void CPlayer_Attack_Script3::PlayerAniPlay3(Vec3 _Dir)
{
    float angle = atan2(_Dir.x, _Dir.y) * 180.0f / XM_PI;

    if (angle >= -45 && angle < 45)  // 위쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Up3, 10.f, false);
    else if (angle >= 45 && angle < 135)  // 오른쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right3, 10.f, false);
    else if (angle >= -135 && angle < -45)  // 왼쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Right3, 10.f, false);
    else  // 아래쪽
        FlipbookPlayer()->Play((UINT)Player_Ani_State::Atk_Down3, 10.f, false);
}

void CPlayer_Attack_Script3::Tick() {}
void CPlayer_Attack_Script3::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
void CPlayer_Attack_Script3::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
void CPlayer_Attack_Script3::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}

void CPlayer_Attack_Script3::SaveComponent(FILE* _File)
{
    fwrite(&m_AtkDmg, sizeof(int), 1, _File);
    fwrite(&m_AtkRange, sizeof(float), 1, _File);
}

void CPlayer_Attack_Script3::LoadComponent(FILE* _File)
{
    fread(&m_AtkDmg, sizeof(int), 1, _File);
    fread(&m_AtkRange, sizeof(float), 1, _File);

    MousDir = Vec3(0.f, 0.f, 0.f);
}