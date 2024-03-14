#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class AObject;

class DemoGun
{
private:
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring path[2]
	{
		L"../Shader/compiled/VertexShader3.cso",
		L"../Shader/compiled/DPass1.cso",
	};

	std::wstring texturePath = L"..\\Model\\DeferredTestMesh\\";

	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	AObject* teatAssimp;

public:
	DemoGun(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoGun();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

