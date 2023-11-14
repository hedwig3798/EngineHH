#include "KeyManager.h"

KeyManager::KeyManager()
	: keyInfomation()
	, hwnd(nullptr)
	, mousePosY(0)
	, mousePosX(0)
	, mouseClickPosX(0)
	, mouseClickPosY(0)
	, mouseInfomation()
	, mouseClick()
{

}

KeyManager::~KeyManager()
{

}

void KeyManager::OnMouseLeftUp(int _x, int _y)
{
	this->mouseClick[(int)MOUSE::LEFT] = false;
}

void KeyManager::OnMouseLeftDown(int _x, int _y)
{
	this->mouseClick[(int)MOUSE::LEFT] = true;
}

void KeyManager::OnMouseRightUp(int _x, int _y)
{
	this->mouseClick[(int)MOUSE::RIGHT] = false;
}

void KeyManager::OnMouseRightDown(int _x, int _y)
{
	this->mouseClick[(int)MOUSE::RIGHT] = true;
}

void KeyManager::OnMouseMove(int _btnState, int _x, int _y)
{
	this->mousePosX = _x;
	this->mousePosY = _y;
}

void KeyManager::Initalize(HWND _hwnd)
{
	hwnd = _hwnd;

	for (UINT i = 0; i < (UINT)KEY::END; i++)
	{
		keyInfomation[i] = tKeyInfo{ KEY_STATE::NONE, false };
	}
}

/// <summary>
/// Ű ���� ������Ʈ
/// ���콺 ��ġ ������Ʈ
/// </summary>
void KeyManager::Update()
{
	// ���� ��Ŀ�� [X] ���¸� Ű ��ü �� ���� ó���ϱ� / ������ �־����� ����
	HWND isGetFocusedHwnd = GetFocus();

	if (isGetFocusedHwnd == nullptr)
	{
		for (size_t i = 0; i < (size_t)KEY::END; i++)
		{
			keyInfomation[i].prevPush = false;

			if (keyInfomation[i].state == KEY_STATE::DOWN || keyInfomation[i].state == KEY_STATE::HOLD)
			{
				keyInfomation[i].state = KEY_STATE::UP;
			}
			if (keyInfomation[i].state == KEY_STATE::UP)
			{
				keyInfomation[i].state = KEY_STATE::NONE;
			}
		}

		for (size_t i = 0; i < (size_t)MOUSE::END; i++)
		{
			mouseInfomation[i].prevPush = false;

			if (mouseInfomation[i].state == KEY_STATE::DOWN || mouseInfomation[i].state == KEY_STATE::HOLD)
			{
				mouseInfomation[i].state = KEY_STATE::UP;
			}
			if (mouseInfomation[i].state == KEY_STATE::UP)
			{
				mouseInfomation[i].state = KEY_STATE::NONE;
			}
		}
		return;
	}

	// ���� ��Ŀ�� [O] �� Ű ���� ������Ʈ
	for (size_t i = 0; i < (size_t)KEY::END; ++i)
	{
		// Ű�� �����ִ�
		if (GetAsyncKeyState(virtualKeyArray[i]) & 0x8000)
		{
			if (keyInfomation[i].prevPush)
			{
				// �������� ���� �־���.
				keyInfomation[i].state = KEY_STATE::HOLD;
			}
			else
			{
				// ������ �������� �ʾҴ�.
				keyInfomation[i].state = KEY_STATE::DOWN;
			}

			keyInfomation[i].prevPush = true;
		}

		// Ű�� �ȴ����ִ�
		else
		{
			if (keyInfomation[i].prevPush)
			{
				// ������ ���� �־���.
				keyInfomation[i].state = KEY_STATE::UP;
			}
			else
			{
				// �������� �������� �ʾҴ�.
				keyInfomation[i].state = KEY_STATE::NONE;
			}

			keyInfomation[i].prevPush = false;
		}
	}

	for (size_t i = 0; i < (size_t)MOUSE::END; i++)
	{
		if (this->mouseClick[i])
		{
			if (this->mouseInfomation[i].prevPush)
			{
				this->mouseInfomation[i].state = KEY_STATE::HOLD;
			}
			else
			{
				this->mouseInfomation[i].state = KEY_STATE::DOWN;
			}
		}
		else
		{
			if (this->mouseInfomation[i].prevPush)
			{
				this->mouseInfomation[i].state = KEY_STATE::UP;
			}
			else
			{
				this->mouseInfomation[i].state = KEY_STATE::NONE;
			}
		}
	}
}

/// <summary>
/// Ű ���� ����
/// </summary>
void KeyManager::Reset()
{
	for (size_t i = 0; i < (size_t)KEY::END; i++)
	{
		keyInfomation[i].state = KEY_STATE::NONE;
		keyInfomation[i].prevPush = false;
	}
}
