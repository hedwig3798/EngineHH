#pragma once
#include "pch.h"


class GraphicsEngine;

class DemoObject
{
private:


public:
	DemoObject();
	~DemoObject();

	void Update(float _dt);
	void Render(GraphicsEngine* ge);
};

