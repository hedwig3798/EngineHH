#pragma once
#include <windows.h>
#include "pch.h"
#include "pipeline.h"

class GraphicsEngine;
class DemoCamera;
class DemoObject;

class DemoProcess
{
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

private:
	// ������ �ڵ鷯
	HWND hwnd;
	GraphicsEngine* graphicsEngine;

	DemoCamera* camera;
	DemoObject* object;
};

