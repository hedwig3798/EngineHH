#include "KeyManager.h"

KeyManager::KeyManager()
	: keyInfomation()
	, hwnd(nullptr)
	, nowMousePosX(0)
	, nowMousePosY(0)
	, mouseInfomation()
	, mouseClick()
	, oldMousePosX(0)
	, oldMousePosY(0)
	, mouseDX(0)
	, mouseDY(0)
{

}

KeyManager::~KeyManager()
{

}

void KeyManager::OnMouseLeftUp(int _x, int _y)
{
	ResetMouse(_x, _y);
	this->mouseClick[(int)MOUSE::LEFT] = false;
}

void KeyManager::OnMouseLeftDown(int _x, int _y)
{
	ResetMouse(_x, _y);
	this->mouseClick[(int)MOUSE::LEFT] = true;
}

void KeyManager::OnMouseRightUp(int _x, int _y)
{
	ResetMouse(_x, _y);
	this->mouseClick[(int)MOUSE::RIGHT] = false;
}

void KeyManager::OnMouseRightDown(int _x, int _y)
{
	ResetMouse(_x, _y);
	this->mouseClick[(int)MOUSE::RIGHT] = true;
}

void KeyManager::OnMouseMove(int _btnState, int _x, int _y)
{
	this->oldMousePosX = this->nowMousePosX;
	this->oldMousePosY = this->nowMousePosY;

	this->nowMousePosX = _x;
	this->nowMousePosY = _y;

	this->mouseDX = this->nowMousePosX - this->oldMousePosX;
	this->mouseDY = this->nowMousePosY - this->oldMousePosY;
}

void KeyManager::Initalize(HWND _hwnd)
{
	hwnd = _hwnd;

	for (UINT i = 0; i < static_cast<UINT>(KEY::END); i++)
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
		for (size_t i = 0; i < static_cast<size_t>(KEY::END); i++)
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

		for (size_t i = 0; i < static_cast<size_t>(MOUSE::END); i++)
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
	for (size_t i = 0; i < static_cast<size_t>(KEY::END); ++i)
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
		for (size_t i = 0; i < static_cast<size_t>(MOUSE::END); i++)
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
				this->mouseInfomation[i].prevPush = true;
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
				this->mouseInfomation[i].prevPush = false;
			}
		}
	}
}

/// <summary>
/// Ű ���� ����
/// </summary>
void KeyManager::Reset()
{
	for (size_t i = 0; i < static_cast<size_t>(KEY::END); i++)
	{
		keyInfomation[i].state = KEY_STATE::NONE;
		keyInfomation[i].prevPush = false;
	}
}

void KeyManager::ResetMouse(int _x /*= 0*/, int _y /*= 0*/)
{
	this->nowMousePosX = _x;
	this->oldMousePosX = _x;

	this->nowMousePosY = _y;
	this->oldMousePosY = _y;

	this->mouseDX = 0;
	this->mouseDY = 0;
}

