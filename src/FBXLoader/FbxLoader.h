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
