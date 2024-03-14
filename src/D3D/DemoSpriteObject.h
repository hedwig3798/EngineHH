#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class UObject;

class DemoSpriteObject
{
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring texturePath = L"..\\Resource\\Texture\\Animation2D\\meow.png";

	ManagerSet* managers;

	UObject* testMesh;

	bool isLocal;
	float aniTime;

public:
	DemoSpriteObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoSpriteObject();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

