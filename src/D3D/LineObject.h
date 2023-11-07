#pragma once
#include "pch.h"
class GraphicsEngine;
class DemoProcess;

class LineObject
{
private:
	VertexC::Data vertexes[100];

	UINT indexes[40];

	PipeLine pipeline;

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/VertexShader.hlsl",
		L"../Shader/PixelShader.hlsl",
	};

	Material demoMat;

public:
	LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene);
	~LineObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

