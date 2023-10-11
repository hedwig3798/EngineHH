#pragma once
#include <windows.h>
#include "pch.h"
#include "pipeline.h"

class GraphicsEngine;
class DemoCamera;
class DemoObject;
class ManagerSet;
class LineObject;
class ModelParser;
class Axes;

class DemoProcess
{
private:
	// 윈도우 핸들러
	HWND hwnd;
	GraphicsEngine* graphicsEngine;

	ManagerSet* managers;

	DemoCamera* camera;
	DemoObject* object;

	LineObject* line;
	Axes* axes;


public:
	// 생성자 소멸자
	DemoProcess();
	~DemoProcess();

	// 명시적 초기화
	void Initialize(HWND _hwnd);

	// 루프에서 실행되는 함수
	void Process();

	DemoCamera* getCamera();

private:
	// 실제 수행되는 함수
	void Update();
	void Render();

};

