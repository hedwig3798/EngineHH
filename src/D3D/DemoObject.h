#pragma once
#include "pch.h"

class GraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class FMesh;

class DemoObject
{
private:
	std::vector<RenderObject*> gemoObject;
	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/VertexShader3.hlsl",
		L"../Shader/PixelShader.hlsl",
	};

	std::wstring texturePath = L"../Model/RainbowTree_Texture01.dds";

	Material demoMat;
	DirectionalLight dirLights[3];
	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	FMesh* testFMesh;

public:
	DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

