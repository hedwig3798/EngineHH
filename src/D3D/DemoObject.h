#pragma once
#include "pch.h"

class GraphicsEngine;
class DemoProcess;

class DemoObject
{
private:
	VertexT::Data* vertexes;
	int vertexesSize;
	UINT* indexes;
	int indexesSize;

	PipeLine pipeline;

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/VertexShader2.hlsl",
		L"../Shader/PixelShader2.hlsl",
	};

	std::wstring texturePath = L"../Model/Texture1.dds";

public:
	DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene);
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

