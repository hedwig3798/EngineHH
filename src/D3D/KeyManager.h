#pragma once
#include "pch.h"
#include <windows.h>
/// <summary>
/// 키 입력 처리를 위한 매니저 클래스
/// 최초 작성일 : 2023/07/21
/// 최초 작성일 : 2023/09/26
/// 작성자 : 김형환
/// </summary>

enum class KEY_STATE
{
	NONE, // 키가 눌리지 않은 상태
	DOWN, // 키가 눌려진 상태 
	HOLD, // 키가 눌린 상태
	UP,	// 키가 올라온 상태
	END,
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	N_1,
	N_2,
	N_3,
	N_4,
	N_5,
	N_6,
	N_7,
	N_8,
	N_9,
	N_0,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,
	LBTN,
	RBTN,
	F_1,
	BACKSPACE,
	END,
};
enum class MOUSE
{
	LEFT,
	RIGHT,
	END
};
struct tKeyInfo
{
	KEY_STATE state;
	bool prevPush;
};

/// <summary>
/// Key 입력을 처리하는 매니저
/// GetKeyState를 쓸 수 있게 한다 
/// </summary>

class KeyManager
{
private:
	HWND hwnd;

	tKeyInfo keyInfomation[(int)KEY::END];
	bool mouseClick[(int)MOUSE::END];
	tKeyInfo mouseInfomation[(int)MOUSE::END];

	int virtualKeyArray[(int)KEY::END] =
	{
		VK_LEFT,	//LEFT,
		VK_RIGHT,	//RIGHT,
		VK_UP,		//UP,
		VK_DOWN,	//DOWN,
		'Q',
		'W',
		'E',
		'R',
		'T',
		'Y',
		'U',
		'I',
		'O',
		'P',
		'A',
		'S',
		'D',
		'F',
		'G',
		'H',
		'J',
		'K',
		'L',
		'Z',
		'X',
		'C',
		'V',
		'B',
		'N',
		'M',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'0',

		VK_MENU,	// ALT
		VK_CONTROL,	// CTRL
		VK_LSHIFT,	// LSHIFT
		VK_SPACE,	// SPACE BAR
		VK_RETURN,	// ENTER
		VK_ESCAPE,	// ESC

		VK_LBUTTON,
		VK_RBUTTON,
		VK_F1,
		VK_BACK
	};

	int mousePosX;
	int mousePosY;

	int mouseClickPosX;
	int mouseClickPosY;

public:
	KeyManager();
	~KeyManager();

	void OnMouseLeftUp(int _x, int _y);
	void OnMouseLeftDown(int _x, int _y);
	void OnMouseRightUp(int _x, int _y);
	void OnMouseRightDown(int _x, int _y);
	void OnMouseMove(int _btnState, int _x, int _y);

	void Initalize(HWND _hwnd);

	void Update();
	void Reset();

	KEY_STATE GetKeyState(KEY _eKey) { return keyInfomation[(int)_eKey].state; }
	KEY_STATE GetMouseState(MOUSE _eKey) { return mouseInfomation[(int)_eKey].state; }
};
