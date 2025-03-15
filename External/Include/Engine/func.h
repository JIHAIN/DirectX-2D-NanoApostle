#pragma once


void CreateObject(class CGameObject* _NewObj, int LayerIdx, bool _bChildMove);
void DestroyObject(CGameObject* _TargetObj);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class CLevel* _Level, LEVEL_STATE _NextState);
void AddChild(CGameObject* _Parent, CGameObject* _Child);


void DrawDebugRect(Vec4 _Color, Vec3 _Pos, Vec2 _Scale, Vec3 _Rotation, bool _DepthTest, float _Duration);
void DrawDebugRect(Vec4 _Color, const Matrix& _matWorld, bool _DepthTest, float _Duration);
void DrawDebugCircle(Vec4 _Color, Vec3 _Pos, float _Radius, bool _DepthTest, float _Duration);
void DrawDebugMouse(Vec4 _Color, Vec3 _Pos, Vec2 _Scale, Vec3 _Rotation, bool _DepthTest, float _Duration);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

#include "Ptr.h"
#include "CAsset.h"
#include "CAssetMgr.h"
#include "singleton.h"

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// ���¿� ���� ���� ������ ����
	bool bAsset = _Asset.Get();
	fwrite(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Key, Path;
		LoadWString(Key, _File);
		LoadWString(Path, _File);

		_Asset = CAssetMgr::GetInst()->Load<T>(Key, Path);
	}
}

template<typename T>
T* LoadAssetRef(FILE* _File)
{
	Ptr<T> pAsset = nullptr;

	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Key, Path;
		LoadWString(Key, _File);
		LoadWString(Path, _File);
		pAsset = CAssetMgr::GetInst()->Load<T>(Key, Path);
	}

	return pAsset.Get();
}







template<typename T, int Count>
void DeleteArray(T* (&arr)[Count])
{
	for (UINT i = 0; i < Count; ++i)
	{
		if (arr[i] == nullptr)
			continue;

		DELETE(arr[i]);
	}
}

template<typename T>
void DeleteVec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (_vec[i] == nullptr)
			continue;

		DELETE(_vec[i]);
	}
	_vec.clear();
}

template<typename T1, typename T2>
void DeleteMap(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}


