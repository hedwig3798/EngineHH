#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class AObject;

class DemoBackObject
{
private:
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;


	ManagerSet* managers;

	bool isLocal;


public:
	AObject* teatAssimp;

	DemoBackObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoBackObject();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

