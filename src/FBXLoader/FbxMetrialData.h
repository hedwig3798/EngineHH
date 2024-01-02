#pragma once
#include "pch.h"

class FbxMetrialData
{
public:
	UINT index;
	std::string ambientFile;
	std::string emissiveFile;
	std::string diffuseFile;
	std::string specularFile;
	std::string normalMapFile;


public:
	FbxMetrialData();
	FbxMetrialData(UINT _index, FbxSurfaceMaterial* _matData);
	~FbxMetrialData();
};

