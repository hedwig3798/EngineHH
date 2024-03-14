#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class FObject;
class AObject;
class DemoMap
{
private:
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring texturePath = L"..\\Model\\DeferredTestMesh\\";

	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	FObject* testFMesh;
	AObject* teatAssimp;

	std::vector<AObject*> testMap;
	std::vector<AObject*> testMap2;
	std::vector<AObject*>& testMapRender;
	float aniTime;

public:
	AObject* testBall;

	DemoMap(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoMap();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

