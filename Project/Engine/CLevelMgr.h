#pragma once
#include "singleton.h"

#include <functional>

class CLevel;

class CLevelMgr :
    public singleton<CLevelMgr>
{
    SINGLE(CLevelMgr);
private:
    CLevel*     m_CurLevel;
    CLevel*     m_Levels[(UINT)LEVEL_TYPE::END];

public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }
    void RegisterObject(class CGameObject* _Object);
    CGameObject* FindObjectByName(const wstring& _Name);
    void ChangeLevelState(LEVEL_STATE _NextState);
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);

    void AddLevel(LEVEL_TYPE _Type, CLevel* _Level) { m_Levels[(UINT)_Type] = _Level; }
    CLevel* GetLevel(LEVEL_TYPE _Type) {return m_Levels[(UINT)_Type]; }
public:
    void Init();
    void Progress();    


public:
    // �ݹ� ��� �Լ� ����
    void AddLevelLoadCallback(const std::function<void(const wstring&)>& _callback)
    {
        m_LoadCallback = _callback;
    }

    // ���� �ε� ��û �Լ� ����
    void RequestLevelLoad(const wstring& _LevelName)
    {
        try
        {
            if (m_LoadCallback && !_LevelName.empty())
            {
                m_LoadCallback(_LevelName);
            }
        }
        catch (...)
        {
            // ���� ó��
            OutputDebugString(L"Level Load Callback Failed\n");
        }
    }


private:
    std::function<void(const wstring&)> m_LoadCallback;



    friend class CTaskMgr;
};

