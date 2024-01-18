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
	// ������ �ڵ鷯
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
	// ������ �Ҹ���
	DemoProcess();
	~DemoProcess();

	// ����� �ʱ�ȭ
	void Initialize(HWND _hwnd);

	// �������� ����Ǵ� �Լ�
	void Process();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
		WPARAM wParam, LPARAM lParam);

private:
	// ���� ����Ǵ� �Լ�
	void Update();
	void Render();
	void CameraUpdate(float _dt);
};

