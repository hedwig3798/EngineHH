#pragma once
#include "pch.h"
#include "fbxVertex.h"

class GraphicsEngine;
class FbxLoader;
class FbxMetrialData;
struct PipeLine;

class FbxData
{
public:
	std::string name;

	FbxData(int _size = 0);

	std::vector<VertexF::Data> vertexData;
	std::vector<UINT> indexData;

	VertexF::Data* vertexBufferData;
	UINT* indexBufferData;

	DirectX::XMMATRIX localTM;
	DirectX::XMMATRIX globalTM;

	PipeLine* pipeline;

	std::vector<std::string> textureFileName;

	FbxData* parent;
	std::vector<FbxData*> children;

	FbxMetrialData* meterial;
};

