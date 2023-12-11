#pragma once
#include "pch.h"
#include "fbxVertex.h"
class FbxData
{
public:
	FbxData(int _size = 0);

	std::vector<VertexF::Data> vertexData;
	std::vector<UINT> indexData;

	FbxData* parent;
	std::vector<FbxData*> children;

	void ResizeVertex(int _size);
	void ResizeIndex(int _size);

	void ResetData();
};

