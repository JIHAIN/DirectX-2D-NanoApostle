#pragma once

enum class KEY
{
	W,
	S,
	A,
	D,
	Z,
	X,
	V,
	C,
	E,
	Y,

	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	SPACE,
	ENTER,

	LSHIFT,

	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_0,

	LBTN,
	RBTN,
	MBTN,

	END,
};



enum class KEY_STATE
{
	TAP,		// 막 눌린 상태
	PRESSED,	// 계속 눌려있는 상태
	RELEASED,	// 막 뗀 상태
	NONE,		// 눌려있지 않은 상태
};


struct tKeyInfo
{
	KEY_STATE	State;
	bool		PrevPressed;
};


class CKeyMgr
	: public singleton<CKeyMgr>
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;

	Vec2				m_MousePos;		// 현재 마우스 위치
	Vec2				m_MousePrevPos;	// 이전 프레임 마우스 위치
	Vec2				m_MouseDir;		// 마우스 좌표가 이동한 방향

	Vec2				m_MousePosCam; // 화면 움직이는거에 영향을 받는 마우스 클라이언트 좌표


public:
	void Init();
	void Tick();

	KEY_STATE GetKeyState(KEY _Key) { return m_vecKey[(int)_Key].State; }
	Vec2 GetMousePos() { return m_MousePos; }
	Vec2 GetMouseDir() { return m_MouseDir; }
	Vec2 GetMousePosCam() { return m_MousePosCam; }

};

