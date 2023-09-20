#pragma once
#include <windows.h>
#include "pch.h"
#include "pipeline.h"
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
	Vertex tempVertex[8] =
	{
		{DirectX::XMFLOAT3{-0.5f, -0.5f, -0.5f}, COLORS::Cyan},
		{DirectX::XMFLOAT3{-0.5f, 0.5f, -0.5f}, COLORS::Black},
		{DirectX::XMFLOAT3{0.5f, 0.5f, -0.5f}, COLORS::Red},
		{DirectX::XMFLOAT3{0.5f, -0.5f, -0.5f}, COLORS::Green},
		{DirectX::XMFLOAT3{-0.5f, -0.5f, 0.5f}, COLORS::Blue},
		{DirectX::XMFLOAT3{-0.5f, 0.5f, 0.5f}, COLORS::Yellow},
		{DirectX::XMFLOAT3{0.5f, 0.5f, 0.5f}, COLORS::Cyan},
		{DirectX::XMFLOAT3{0.5f, -0.5f, 0.5f}, COLORS::Magenta},
	};

	UINT tempIndex[36] =
	{
		0,1,2,
		0,2,3,
		4,6,5,
		4,7,6,
		4,5,1,
		4,1,0,
		3,2,6,
		3,6,7,
		1,5,6,
		1,6,7,
		4,0,3,
		4,3,7
	};

	PipeLine pipeline;


};

