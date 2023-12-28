#include "FbxData.h"

FbxData::FbxData(int _size /*= 0*/)
	: vertexData(std::vector<VertexF::Data>(_size))
	, indexData(std::vector<UINT>(_size))
	, parent(nullptr)
	, children(std::vector<FbxData*>())
	, vertexBufferData(nullptr)
	, indexBufferData(nullptr)
	, pipeline(nullptr)
	, localTM{}
	, globalTM{}
	, meterial(nullptr)
	, name("")
	, textureFileName{std::vector<std::string>()}
{
}
