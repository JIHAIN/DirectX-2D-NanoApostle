#include "pch.h"
#include "func.h"

#include "CRenderMgr.h"
#include "CTaskMgr.h"


void CreateObject(CGameObject* _NewObj, int LayerIdx, bool _bMoveChild)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param0 = (DWORD_PTR)_NewObj;
	task.Param1 = LayerIdx;
	task.Param2 = _bMoveChild;

	CTaskMgr::GetInst()->AddTask(task);
}

void DestroyObject(CGameObject* _TargetObj)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param0 = (DWORD_PTR)_TargetObj;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	task.Param0 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(CLevel* _Level, LEVEL_STATE _NextState)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)_Level;
	task.Param1 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}

void AddChild(CGameObject* _Parent, CGameObject* _Child)
{
	tTask task = {};

	task.Type = TASK_TYPE::ADD_CHILD;
	task.Param0 = (DWORD_PTR)_Parent;
	task.Param1 = (DWORD_PTR)_Child;

	CTaskMgr::GetInst()->AddTask(task);
}

void DrawDebugRect(Vec4 _Color, Vec3 _Pos, Vec2 _Scale, Vec3 _Rotation, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;

	info.WorldPos = _Pos;
	info.Scale = Vec3(_Scale.x, _Scale.y, 1.f);
	info.Rotation = _Rotation;
	info.matWorld = XMMatrixIdentity();

	info.DepthTest = _DepthTest;
	info.Duration = _Duration;
	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugRect(Vec4 _Color, const Matrix& _matWorld, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;

	info.matWorld = _matWorld;

	info.DepthTest = _DepthTest;
	info.Duration = _Duration;
	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugCircle(Vec4 _Color, Vec3 _Pos, float _Radius, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CIRCLE;
	info.Color = _Color;
	info.WorldPos = _Pos;
	info.Scale = Vec3(_Radius, _Radius, 1.f);
	info.DepthTest = _DepthTest;
	info.Duration = _Duration;
	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugMouse(Vec4 _Color, Vec3 _Pos, Vec2 _Scale, Vec3 _Rotation, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::MOUSE;
	info.Color = _Color;

	info.WorldPos = _Pos;
	info.Scale = Vec3(_Scale.x, _Scale.y, 1.f);
	info.Rotation = _Rotation;
	info.matWorld = XMMatrixIdentity();

	info.DepthTest = _DepthTest;
	info.Duration = _Duration;
	CRenderMgr::GetInst()->AddDebugShape(info);
}



void SaveWString(const wstring& _str, FILE* _File)
{
	size_t Len = _str.length();
	fwrite(&Len, sizeof(size_t), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), Len, _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[255] = {};

	size_t Len = 0;
	fread(&Len, sizeof(size_t), 1, _File);
	fread(szBuffer, sizeof(wchar_t), Len, _File);
	_str = szBuffer;
}



