#pragma once
#include "pch.h"
#include "fbxVertex.h"

class GraphicsEngine;
class FbxLoader;
struct PipeLine;

class FbxData
{
public:
	FbxData(int _size = 0);

	std::vector<VertexF::Data> vertexData;
	std::vector<UINT> indexData;

	VertexF::Data* vertexBufferData;
	UINT* indexBufferData;

	DirectX::XMMATRIX localTM;
	DirectX::XMMATRIX globalTM;

	PipeLine* pipeline;

	FbxData* parent;
	std::vector<FbxData*> children;

	std::vector<DirectX::XMFLOAT3> position;
	std::vector<DirectX::XMFLOAT3> noraml;
	std::vector<DirectX::XMFLOAT2> UV;
	std::vector<DirectX::XMFLOAT3> binormal;
	std::vector<DirectX::XMFLOAT3> tangent;
};

