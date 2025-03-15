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
	TAP,		// �� ���� ����
	PRESSED,	// ��� �����ִ� ����
	RELEASED,	// �� �� ����
	NONE,		// �������� ���� ����
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

	Vec2				m_MousePos;		// ���� ���콺 ��ġ
	Vec2				m_MousePrevPos;	// ���� ������ ���콺 ��ġ
	Vec2				m_MouseDir;		// ���콺 ��ǥ�� �̵��� ����

	Vec2				m_MousePosCam; // ȭ�� �����̴°ſ� ������ �޴� ���콺 Ŭ���̾�Ʈ ��ǥ


public:
	void Init();
	void Tick();

	KEY_STATE GetKeyState(KEY _Key) { return m_vecKey[(int)_Key].State; }
	Vec2 GetMousePos() { return m_MousePos; }
	Vec2 GetMouseDir() { return m_MouseDir; }
	Vec2 GetMousePosCam() { return m_MousePosCam; }

};

