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
//    bool            hit;            // �浹 ����
//    float           distance;       // �浹 ���������� �Ÿ�
//    Vec2            point;          // �浹 ����
//    Vec2            normal;         // �浹 ������ ���� ����
//    CGameObject*    hitObject;      // �浹�� ������Ʈ
//    CCollider2D*    hitCollider;    // �浹�� �ݶ��̴�
//};


class CCollider2D;

class CCollisionMgr :
    public singleton<CCollisionMgr>
{
    SINGLE(CCollisionMgr)
private:
    UINT                    m_Matrix[MAX_LAYER];
    map<ULONGLONG, bool>    m_ColInfo;

//public: // ����ĳ��Ʈ ��
//
//    // ���� ����ĳ��Ʈ
//    RaycastHit2D Raycast(Vec2 _origin, Vec2 _direction, float _maxDistance, UINT _layerMask = 0xFFFFFFFF);
//    // Ư�� ���� ���� Ÿ�� ã�� (���� ���� ��ɿ�)
//    CGameObject* FindTargetInAngle(Vec2 _origin, Vec2 _forward, float _angle, float _maxDistance, UINT _layerMask);
//    // ���̿� AABB �浹 �˻�
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

