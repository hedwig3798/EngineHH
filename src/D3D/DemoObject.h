#pragma once
#include "pch.h"

class GraphicsEngine;
class DemoProcess;

class DemoObject
{
private:
	Vertex* vertexes;
	int vertexesSize;
	UINT* indexes;
	int indexesSize;

	PipeLine pipeline;

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

public:
	DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene);
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

