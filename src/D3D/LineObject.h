#pragma once
#include "pch.h"
class GraphicsEngine;
class DemoProcess;

class LineObject
{
private:
	Vertex vertexes[2];

	UINT indexes[2] =
	{
		0, 1
	};

	PipeLine pipeline;

	GraphicsEngine* graphicsEngine;

	DemoProcess* scene;

public:
	LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, Vertex _start, Vertex _end);
	~LineObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

