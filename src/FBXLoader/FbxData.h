#pragma once
#include "pch.h"
#include "fbxVertex.h"

class GraphicsEngine;
class FbxLoader;
class FbxMetrialData;
struct PipeLine;
class FbxBone;
class FbxMeshData;

class FbxData
{
public:
	std::string name;

	FbxMeshData* mesh;
	
	std::vector<FbxBone*> boneData;


	FbxMetrialData* meterial;

	FbxData();
};

