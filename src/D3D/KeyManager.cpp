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
/// 키 상태 업데이트
/// 마우스 위치 업데이트
/// </summary>
void KeyManager::Update()
{
	// 게임 포커스 [X] 상태면 키 전체 안 누름 처리하기 / 누르고 있었으면 떼기
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

	// 게임 포커스 [O] 때 키 상태 업데이트
	for (size_t i = 0; i < static_cast<size_t>(KEY::END); ++i)
	{
		// 키가 눌려있다
		if (GetAsyncKeyState(virtualKeyArray[i]) & 0x8000)
		{
			if (keyInfomation[i].prevPush)
			{
				// 이전에도 눌려 있었다.
				keyInfomation[i].state = KEY_STATE::HOLD;
			}
			else
			{
				// 이전에 눌려있지 않았다.
				keyInfomation[i].state = KEY_STATE::DOWN;
			}

			keyInfomation[i].prevPush = true;
		}

		// 키가 안눌려있다
		else
		{
			if (keyInfomation[i].prevPush)
			{
				// 이전에 눌려 있었다.
				keyInfomation[i].state = KEY_STATE::UP;
			}
			else
			{
				// 이전에도 눌려있지 않았다.
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
/// 키 상태 리셋
/// </summary>
void KeyManager::Reset()
{
	for (size_t i = 0; i < static_cast<size_t>(KEY::END); i++)
	{
		keyInfomation[i].state = KEY_STATE::NONE;
		keyInfomation[i].prevPush = false;
	}
}
