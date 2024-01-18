#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class FObject;

class DemoObject
{
private:
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/compiled/VertexShader3.cso",
		L"../Shader/compiled/DPass1.cso",
	};

	std::wstring texturePath = L"../Model/";

	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	FObject* testFMesh;

public:
	DemoObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoObject();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

