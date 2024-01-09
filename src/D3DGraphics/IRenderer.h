#pragma once
#include "pch.h"

class GraphicsEngine;

class IRenderer abstract
{
public:
	IRenderer() {};
	virtual ~IRenderer() {};

	virtual void Initailze() abstract;
	virtual void BeginRender(GraphicsEngine* _gp) abstract;
	virtual void Updata(float _dt) abstract;
};

