#pragma once
#include "pch.h"

class GraphicsEngine;
class PipeLine;

class IRenderer abstract
{
public:
	GraphicsEngine* gp = nullptr;

	virtual ~IRenderer() {};

	virtual void Initailze(GraphicsEngine* _gp) abstract;
	
	virtual void BeginRender() abstract;
	virtual void EndRender() abstract;

	virtual void BindPipeline(PipeLine* _pipeline) abstract;

	virtual void BindVSResource() abstract;
	virtual void BindPSResource() abstract;

	virtual void SetVSArgument() abstract;
	virtual void SetPSArgument() abstract;

	virtual void BindWVPMatrix() abstract;
	virtual void BindLight() abstract;
	virtual void BindBone() abstract;
	virtual void WriteText() abstract;
};

