#pragma once


union COLLIDER_ID
{
    struct
    {
        UINT Left;
        UINT Right;
    };

    ULONGLONG ID;
};

//struct RaycastHit2D
//{
//    bool            hit;            // 충돌 여부
//    float           distance;       // 충돌 지점까지의 거리
//    Vec2            point;          // 충돌 지점
//    Vec2            normal;         // 충돌 지점의 법선 벡터
//    CGameObject*    hitObject;      // 충돌한 오브젝트
//    CCollider2D*    hitCollider;    // 충돌한 콜라이더
//};


class CCollider2D;

class CCollisionMgr :
    public singleton<CCollisionMgr>
{
    SINGLE(CCollisionMgr)
private:
    UINT                    m_Matrix[MAX_LAYER];
    map<ULONGLONG, bool>    m_ColInfo;

//public: // 레이캐스트 용
//
//    // 단일 레이캐스트
//    RaycastHit2D Raycast(Vec2 _origin, Vec2 _direction, float _maxDistance, UINT _layerMask = 0xFFFFFFFF);
//    // 특정 각도 내의 타겟 찾기 (조준 보조 기능용)
//    CGameObject* FindTargetInAngle(Vec2 _origin, Vec2 _forward, float _angle, float _maxDistance, UINT _layerMask);
//    // 레이와 AABB 충돌 검사
//    bool RayToAABB(const Vec2& _rayOrigin, const Vec2& _rayDir,
//        const Matrix& _colliderWorld, float& _distance);


public:
    void CollisionCheck(UINT _Left, UINT _Right);
    void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * MAX_LAYER); }

private:
    void CollisionBtwLayer(UINT _Left, UINT _Right);
    void CollisionBtwCollider2D(CCollider2D* _LeftCol, CCollider2D* _RightCol);
    bool IsCollision(CCollider2D* _Left, CCollider2D* _Right);


public:
    void Tick();
};

