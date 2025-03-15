#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::Init()
{
	// 현재 작업 디렉터리를 알아낸다. (실행시킨 *.exe 파일 위치 OR 프로젝트 작업디렉터리 위치)
	wchar_t szCurDir[256];
	GetCurrentDirectory(256, szCurDir);

	int Len = wcslen(szCurDir);

	for (int i = Len - 1; 0 < i; --i)
	{
		if (szCurDir[i] == L'\\')
		{
			szCurDir[i] = L'\0';
			break;
		}
	}

	m_ContentPath = szCurDir;
	m_ContentPath += L"\\Content\\";

	m_BinPath = szCurDir;
	m_BinPath += L"bin\\";
}
