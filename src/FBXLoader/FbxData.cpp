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
	, position(std::vector<DirectX::XMFLOAT3>())
	, noraml(std::vector<DirectX::XMFLOAT3>())
	, UV(std::vector<DirectX::XMFLOAT2>())
	, binormal(std::vector<DirectX::XMFLOAT3>())
	, tangent(std::vector<DirectX::XMFLOAT3>())
{
}
