#pragma once
#include "pch.h"

class FbxData;
class GraphicsEngine;

class FMesh
{
public:
	FbxData* fData;
	
public:
	FMesh();
	~FMesh();

	void Render(GraphicsEngine* _gp);
};

