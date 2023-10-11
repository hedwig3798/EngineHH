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
	// ������ �ڵ鷯
	HWND hwnd;
	GraphicsEngine* graphicsEngine;

	ManagerSet* managers;

	DemoCamera* camera;
	DemoObject* object;

	LineObject* line;
	Axes* axes;


public:
	// ������ �Ҹ���
	DemoProcess();
	~DemoProcess();

	// ����� �ʱ�ȭ
	void Initialize(HWND _hwnd);

	// �������� ����Ǵ� �Լ�
	void Process();

	DemoCamera* getCamera();

private:
	// ���� ����Ǵ� �Լ�
	void Update();
	void Render();

};

