#pragma once
#include "pch.h"
class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class Mesh;
class RenderObject;
class AObject;


class DemoBoxObject
{
private:
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	UINT lightCount;
	ManagerSet* managers;

	bool isLocal;

	AObject* teatAssimp;

	float aniTime;

public:
	DemoBoxObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoBoxObject();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

