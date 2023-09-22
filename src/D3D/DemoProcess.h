#pragma once
#include <windows.h>
#include "pch.h"
#include "pipeline.h"
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
	Vertex tempVertex[8] =
	{
		{DirectX::XMFLOAT3{-1.0f, -1.0f, -1.0f}, COLORS::Cyan},
		{DirectX::XMFLOAT3{-1.0f, 1.0f, -1.0f}, COLORS::Black},
		{DirectX::XMFLOAT3{1.0f, 1.0f, -1.0f}, COLORS::Red},
		{DirectX::XMFLOAT3{1.0f, -1.0f, -1.0f}, COLORS::Green},
		{DirectX::XMFLOAT3{-1.0f, -1.0f, 1.0f}, COLORS::Blue},
		{DirectX::XMFLOAT3{-1.0f, 1.0f, 1.0f}, COLORS::Yellow},
		{DirectX::XMFLOAT3{1.0f, 1.0f, 1.0f}, COLORS::Cyan},
		{DirectX::XMFLOAT3{1.0f, -1.0f, 1.0f}, COLORS::Magenta},
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
		1,6,2,

		4,0,3,
		4,3,7
	};

	PipeLine pipeline;


};

