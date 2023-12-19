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
	DirectX::XMMATRIX FbxAMatrixToXMMatrix(const FbxAMatrix& _pSrc);

	DirectX::XMFLOAT3 ReadNormal(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter);
	DirectX::XMFLOAT3 ReadBinormal(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter);
	DirectX::XMFLOAT3 ReadTangent(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter);
	DirectX::XMFLOAT2 ReadUV(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter);

	template<typename T>
	DirectX::XMFLOAT3 ReadThing(const T* _thigs, int _controlPointIndex, int _vertexCounter);
};

template<typename T>
DirectX::XMFLOAT3 FbxLoader::ReadThing(const T* _thigs, int _controlPointIndex, int _vertexCounter)
{
	DirectX::XMFLOAT3 result = {};

	int counter;
	switch (_thigs->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		counter = _controlPointIndex;
		break;
	}
	case FbxGeometryElement::eByPolygonVertex:
	{
		counter = _vertexCounter;
		break;
	}
	default:
		assert(false && "cannot access FbxGeometryElementNormal");
		break;
	}

	switch (_thigs->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
	{
		result.x = static_cast<float>(_thigs->GetDirectArray().GetAt(counter).mData[0]);
		result.y = static_cast<float>(_thigs->GetDirectArray().GetAt(counter).mData[1]);
		result.z = static_cast<float>(_thigs->GetDirectArray().GetAt(counter).mData[2]);
		break;
	}
	case FbxGeometryElement::eIndexToDirect:
	{
		int index = _thigs->GetIndexArray().GetAt(counter); // 인덱스를 얻어온다.
		result.x = static_cast<float>(_thigs->GetDirectArray().GetAt(index).mData[0]);
		result.y = static_cast<float>(_thigs->GetDirectArray().GetAt(index).mData[1]);
		result.z = static_cast<float>(_thigs->GetDirectArray().GetAt(index).mData[2]);
		break;
	}
	default:
		break;
	}

	return result;
}

// struct hash_fn
// {
// 	std::size_t operator() (const VertexF::Data& _data) const
// 	{
// 		std::size_t h1 = std::hash<float>()(_data.position.x);
// 		h1 ^= std::hash<float>()(_data.position.y);
// 		h1 ^= std::hash<float>()(_data.position.z);
// 
// 		h1 ^= std::hash<float>()(_data.normal.x);
// 		h1 ^= std::hash<float>()(_data.normal.y);
// 		h1 ^= std::hash<float>()(_data.normal.z);
// 
// 		h1 ^= std::hash<float>()(_data.binormal.x);
// 		h1 ^= std::hash<float>()(_data.binormal.y);
// 		h1 ^= std::hash<float>()(_data.binormal.z);
// 
// 		h1 ^= std::hash<float>()(_data.tangent.x);
// 		h1 ^= std::hash<float>()(_data.tangent.y);
// 		h1 ^= std::hash<float>()(_data.tangent.z);
// 
// 		h1 ^= std::hash<float>()(_data.UV.x);
// 		h1 ^= std::hash<float>()(_data.UV.y);
// 		return h1;
// 	}
// };
