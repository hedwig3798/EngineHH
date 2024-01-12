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
		L"../Shader/compiled/VertexShader.cso",
		L"../Shader/compiled/PixelShader.cso",
	};

	Material demoMat;

public:
	LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene);
	~LineObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

