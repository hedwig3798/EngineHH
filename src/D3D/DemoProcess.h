#pragma once
#include <windows.h>

class GraphicsEngine;

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

private:
	// ���� ����Ǵ� �Լ�
	void Update();
	void Render();

private:
	// ������ �ڵ鷯
	HWND hwnd;
	GraphicsEngine* graphicsEngine;
};

