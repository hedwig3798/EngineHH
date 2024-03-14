#pragma once

#include "pch.h"

class IGraphicsEngine;
class DemoObject;
class DemoBoxObject;
class DemoBackObject;
class DemoUIObject;
class DemoSpriteObject;
class DemoDog;
class DemoGun;
class ManagerSet;
class LineObject;
class ModelParser;
class Axes;
class ICamera;
class DemoMap;
class TObject;

class DemoProcess
{
private:
	// ������ �ڵ鷯
	HWND hwnd;
	IGraphicsEngine* graphicsEngine;

	ManagerSet* managers;
	static ManagerSet* staticManagers;

	DemoObject* object;
	DemoBoxObject* object2;
	DemoBackObject* objectBack;
	DemoDog* objectDog;
	DemoUIObject* objectUI;
	DemoSpriteObject* objectSprit;
	DemoMap* demoMap;
	TObject* tobject;

	DemoGun* object3;
	
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

	void EffectOnOff();
};

