#include "FbxMeshData.h"

FbxMeshData::FbxMeshData()
	: fbxData(nullptr)
	, indexData(std::vector<UINT>())
	, parent(nullptr)
	, children(std::vector<FbxMeshData*>())
	, vertexBufferData(nullptr)
	, indexBufferData(nullptr)
	, pipeline(nullptr)
	, localTM{}
	, globalTM{}
	, name("")
{
}

FbxMeshData::~FbxMeshData()
{
}
