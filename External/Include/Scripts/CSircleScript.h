#pragma once
#include <Engine/CScript.h>

class CSircleScript :
    public CScript
{
private:
    int m_Type; // 1.�Ʒ��� 2. ���� 3. �����¿� �װ� 4. ���������� 

public:
    virtual void Tick() override;
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void CreateAtk(Vec3 _Pos);
    int GetSircleType() { return m_Type; }
public:
    CLONE(CSircleScript);
    CSircleScript();
    ~CSircleScript();
};

