#pragma once
#include "CAsset.h"


class CGameObject;

class CPrefab :
    public CAsset
{
public:
    static GAMEOBJECT_SAVE g_ObjectSave;
    static GAMEOBJECT_LOAD g_ObjectLoad;

private:
    CGameObject* m_ProtoObj;

public:
    void SetProtoObject(CGameObject* _Object);
    CGameObject* Instantiate();
    void CreatePrefab(wstring _Name, Vec3 _RelativePos, int _Layer);

    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

public:
    CLONE(CPrefab);
    CPrefab();
    CPrefab(const CPrefab& _Origin);
    ~CPrefab();
};