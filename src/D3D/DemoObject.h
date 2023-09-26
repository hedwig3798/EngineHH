#pragma once
#include "pch.h"

class GraphicsEngine;
class DemoProcess;

class DemoObject
{
private:
	Vertex vertexes[8] =
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

	UINT indexes[36] =
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

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

public:
	DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene);
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

