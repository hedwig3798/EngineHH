#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "TimeManager.h"

class KeyManager;
class TimeManager;

class ManagerSet
{
public:
	ManagerSet();
	~ManagerSet();

	KeyManager* keyManager;
	TimeManager* timeManager;

	void Initialize(HWND _hwnd);
	void Update();
};

