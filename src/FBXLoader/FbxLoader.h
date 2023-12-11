#pragma once
#include "pch.h"
#include "FbxData.h"

class FbxLoader
{
public:
	FbxLoader();

	FbxManager* fbxManager;
	FbxImporter* fbxImpoter;
	FbxScene* fbxScene;
	FbxGeometryConverter* geometryConverter;

	void Initalize();
	void Release();

	FbxData* Load(std::string _path);

private:
	void LoadNode(FbxNode* _parent, FbxData* _data);
};

