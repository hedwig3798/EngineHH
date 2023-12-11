#include "FbxData.h"

FbxData::FbxData(int _size /*= 0*/)
	: vertexData(std::vector<VertexF::Data>(_size))
	, indexData(std::vector<UINT>(_size))
	, parent(nullptr)
	, children(std::vector<FbxData*>())
{

}

void FbxData::ResizeVertex(int _size)
{
	this->vertexData.resize(_size);
}

void FbxData::ResizeIndex(int _size)
{
	this->indexData.resize(_size);
}

void FbxData::ResetData()
{
	this->vertexData = std::vector<VertexF::Data>();
	this->indexData = std::vector<UINT>();
}
