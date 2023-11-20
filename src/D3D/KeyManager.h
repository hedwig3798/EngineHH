#pragma once
#include "pch.h"
#include <windows.h>
/// <summary>
/// Ű �Է� ó���� ���� �Ŵ��� Ŭ����
/// ���� �ۼ��� : 2023/07/21
/// ���� �ۼ��� : 2023/09/26
/// �ۼ��� : ����ȯ
/// </summary>

enum class KEY_STATE
{
	NONE, // Ű�� ������ ���� ����
	DOWN, // Ű�� ������ ���� 
	HOLD, // Ű�� ���� ����
	UP,	// Ű�� �ö�� ����
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
/// Key �Է��� ó���ϴ� �Ŵ���
/// GetKeyState�� �� �� �ְ� �Ѵ� 
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

	int nowMousePosX;
	int nowMousePosY;

	int oldMousePosX;
	int oldMousePosY;


public:
	KeyManager();
	~KeyManager();

	int mouseDX;
	int mouseDY;
	void OnMouseLeftUp(int _x, int _y);
	void OnMouseLeftDown(int _x, int _y);
	void OnMouseRightUp(int _x, int _y);
	void OnMouseRightDown(int _x, int _y);
	void OnMouseMove(int _btnState, int _x, int _y);

	void Initalize(HWND _hwnd);

	void Update();
	void Reset();

	void ResetMouse(int _x = 0, int _y = 0);

	KEY_STATE GetKeyState(KEY _eKey) { return keyInfomation[(int)_eKey].state; }
	KEY_STATE GetMouseState(MOUSE _eKey) { return mouseInfomation[(int)_eKey].state; }
};
