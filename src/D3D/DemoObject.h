#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class AObject;

class DemoObject
{
private:
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring texturePath = L"..\\Model\\DeferredTestMesh\\";

	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	float aniTime;

public:
	AObject* teatAssimp;
	DemoObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoObject();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

