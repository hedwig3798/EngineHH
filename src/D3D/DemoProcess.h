#pragma once
#include <windows.h>

class GraphicsEngine;

class DemoProcess
{
public:
	// 생성자 소멸자
	DemoProcess();
	~DemoProcess();

	// 명시적 초기화
	void Initialize(HWND _hwnd);

	// 루프에서 실행되는 함수
	void Process();

private:
	// 실제 수행되는 함수
	void Update();
	void Render();

private:
	// 윈도우 핸들러
	HWND hwnd;
	GraphicsEngine* graphicsEngine;
};

