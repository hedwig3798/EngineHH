#pragma once
#include "pch.h"
#include "fbxVertex.h"

class FbxData;
struct PipeLine;

class FbxMeshData
{
public:
	FbxMeshData();
	~FbxMeshData();

	FbxData* fbxData;

	FbxMeshData* parent;
	std::vector<FbxMeshData*> children;

	std::string name;

	std::vector<VertexF::Data> vertexData;
	std::vector<UINT> indexData;
	VertexF::Data* vertexBufferData;
	UINT* indexBufferData;

	DirectX::XMMATRIX localTM;
	DirectX::XMMATRIX globalTM;

	std::vector<std::string> textureFileName;

	PipeLine* pipeline;
};

