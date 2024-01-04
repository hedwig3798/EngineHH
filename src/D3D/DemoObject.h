#pragma once
#include "pch.h"

class GraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class FObject;

class DemoObject
{
private:
	std::vector<RenderObject*> gemoObject;
	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/compiled/VertexShader3.cso",
		L"../Shader/compiled/PixelShader3.cso",
	};

	std::wstring texturePath = L"../Model/";

	Material demoMat;
	DirectionalLight dirLights[3];
	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	FObject* testFMesh;

public:
	DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

