#pragma once

#include "pch.h"

class IGraphicsEngine;
class DemoObject;
class ManagerSet;
class LineObject;
class ModelParser;
class Axes;
class ICamera;

class DemoProcess
{
private:
	// 윈도우 핸들러
	HWND hwnd;
	IGraphicsEngine* graphicsEngine;

	ManagerSet* managers;
	static ManagerSet* staticManagers;

	DemoObject* object;

	LineObject* line;
	Axes* axes;

	std::wstring explain;

	ICamera* camera;

public:
	// 생성자 소멸자
	DemoProcess();
	~DemoProcess();

	// 명시적 초기화
	void Initialize(HWND _hwnd);

	// 루프에서 실행되는 함수
	void Process();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
		WPARAM wParam, LPARAM lParam);

private:
	// 실제 수행되는 함수
	void Update();
	void Render();
	void CameraUpdate(float _dt);
};

