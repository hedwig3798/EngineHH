#pragma once
#include "pch.h"

class GraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;

class DemoObject
{
private:
	VertexT::Data* vertexes;
	int vertexesSize;
	UINT* indexes;
	int indexesSize;

	std::vector<Mesh*> meshies;

	PipeLine pipeline;

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/VertexShader2.hlsl",
		L"../Shader/PixelShader2.hlsl",
	};

	std::wstring texturePath = L"../Model/Texture1.dds";

	Material demoMat;
	DirectionalLight dirLights[3];
	UINT lightCount;
	ManagerSet* managers;
public:
	DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

