#pragma once
#include "pch.h"
class GraphicsEngine;
class DemoProcess;

class LineObject
{
private:
	VertexC::Data vertexes[2];

	UINT indexes[2] =
	{
		0, 1
	};

	PipeLine pipeline;

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/VertexShader.hlsl",
		L"../Shader/PixelShader.hlsl",
	};

public:
	LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, VertexC::Data _start, VertexC::Data _end);
	~LineObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

