#include "FbxLoader.h"

FbxLoader::FbxLoader()
	: fbxManager(nullptr)
	, fbxImpoter(nullptr)
	, fbxScene(nullptr)
	, geometryConverter(nullptr)
{

}

void FbxLoader::Initalize()
{
	// Fbx 로드를 위한 클래스 생성
	this->fbxManager = FbxManager::Create();
	assert(this->fbxManager && "cannot create fbx manager\n");

	this->fbxImpoter = FbxImporter::Create(fbxManager, "");
	assert(this->fbxManager && "cannot create fbx manager\n");

	this->fbxScene = FbxScene::Create(fbxManager, "");
	assert(this->fbxScene && "cannot create fbx manager\n");

	// 매쉬를 삼각형으로 잘라줄 클래스 생성
	this->geometryConverter = new FbxGeometryConverter(this->fbxManager);
}

void FbxLoader::Release()
{
	// 미리 생성한 클래스 제거
	this->fbxManager->Destroy();
	this->fbxImpoter->Destroy();
	this->fbxScene->Destroy();
	delete this->geometryConverter;
}

FbxData* FbxLoader::Load(std::string _path)
{
	// 경로를 통해 임포터 클래스 초기화
	bool ret = this->fbxImpoter->Initialize(_path.c_str());
	assert(ret && "cannot initalize fbx importer when load\n");

	// 임포터를 통해 씬 임포트
	ret = this->fbxImpoter->Import(this->fbxScene);
	assert(ret && "cannot import fbx scene when load\n");

	// 씬에서 루트 노드 가져옴
	FbxNode* node = this->fbxScene->GetRootNode();
	assert(node && "cannot load Node from scene\n");

	// 해당 씬의 노드를 삼각형으로 쪼개기
	this->geometryConverter->Triangulate(this->fbxScene, true);

	// 리턴할 데이터 클래스
	FbxData* data = new FbxData();
	// 데이터 클래스에 데이터 로드
	LoadNode(node, data);

	return data;
}

void FbxLoader::LoadNode(FbxNode* _node, FbxData* _data)
{
	// 노드의 속성
	FbxNodeAttribute* nodeAttribute = _node->GetNodeAttribute();

	// 노드의 월드 변환 행렬
	FbxAMatrix& globalTransform = _node->EvaluateGlobalTransform();
	_data->globalTM = FbxAMatrixToXMMatrix(globalTransform);

	// 노드의 로컬 변환 행렬
	FbxAMatrix& localTransform = _node->EvaluateLocalTransform();
	_data->localTM = FbxAMatrixToXMMatrix(localTransform);

	// 노드 속성이 있다면
	if (nodeAttribute != nullptr)
	{
		// 이 노드의 속성은 메쉬.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FbxMesh로 캐스팅된 노드 속성의 포인터를 가져온다.
			fbxsdk::FbxMesh* mesh = _node->GetMesh();

			// 위치 정보가 있는 정점 정보를 가지고 온다.
			int vertexCount = mesh->GetControlPointsCount();
			_data->position.resize(vertexCount);

			// 데이터의 위치 정보에 입력
			for (int i = 0; i < vertexCount; i++)
			{
				_data->position[i].x = (static_cast<float>(mesh->GetControlPointAt(i).mData[0]));
				_data->position[i].y = (static_cast<float>(mesh->GetControlPointAt(i).mData[1]));
				_data->position[i].z = (static_cast<float>(mesh->GetControlPointAt(i).mData[2]));
			}

			// 해당 매쉬가 몇개의 삼각형을 가지고 있는지
			int indexCount = mesh->GetPolygonCount();
			_data->indexData.resize(static_cast<size_t>(indexCount * 3));

			// 삼각형의 갯수 만큼 vertex 생성
			for (int i = 0; i < indexCount; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					int controlPointIndex = mesh->GetPolygonVertex(i, j);

					DirectX::XMFLOAT3& pos = _data->position[controlPointIndex];
					DirectX::XMFLOAT3 normal = ReadNormal(mesh, controlPointIndex, vertexCount);
					DirectX::XMFLOAT3 binormal = ReadBinormal(mesh, controlPointIndex, vertexCount);
					DirectX::XMFLOAT3 tangent = ReadTangent(mesh, controlPointIndex, vertexCount);
				}
			}
		}
	}

	// 노드의 자식들 또한 로드
	for (int i = 0; i < _node->GetChildCount(); ++i)
	{
		FbxData* child = new FbxData();
		_data->children.push_back(child);
		child->parent = _data;
		LoadNode(_node->GetChild(i), child);
	}
}

DirectX::XMMATRIX FbxLoader::FbxAMatrixToXMMatrix(const FbxAMatrix& _fbxMatrix)
{
	// fbx의 행렬 데이터를 Diretx에 맞도록 수정
	return DirectX::XMMatrixSet(
		static_cast<FLOAT>(_fbxMatrix.Get(0, 0)), static_cast<FLOAT>(_fbxMatrix.Get(0, 1)), static_cast<FLOAT>(_fbxMatrix.Get(0, 2)), static_cast<FLOAT>(_fbxMatrix.Get(0, 3)),
		static_cast<FLOAT>(_fbxMatrix.Get(1, 0)), static_cast<FLOAT>(_fbxMatrix.Get(1, 1)), static_cast<FLOAT>(_fbxMatrix.Get(1, 2)), static_cast<FLOAT>(_fbxMatrix.Get(1, 3)),
		static_cast<FLOAT>(_fbxMatrix.Get(2, 0)), static_cast<FLOAT>(_fbxMatrix.Get(2, 1)), static_cast<FLOAT>(_fbxMatrix.Get(2, 2)), static_cast<FLOAT>(_fbxMatrix.Get(2, 3)),
		static_cast<FLOAT>(_fbxMatrix.Get(3, 0)), static_cast<FLOAT>(_fbxMatrix.Get(3, 1)), static_cast<FLOAT>(_fbxMatrix.Get(3, 2)), static_cast<FLOAT>(_fbxMatrix.Get(3, 3)));
}

DirectX::XMFLOAT3 FbxLoader::ReadNormal(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter)
{
	DirectX::XMFLOAT3 result = {};

	if (!(_mesh->GetElementNormalCount() < 1))
	{
		const FbxGeometryElementNormal* vBinormal = _mesh->GetElementNormal(0);
		result = ReadThing<FbxGeometryElementNormal>(vBinormal, _controlPointIndex, _vertexCounter);
	}
	else
	{
		result = { 0.0f, 0.0f, 0.0f };
	}
	return result;
}

DirectX::XMFLOAT3 FbxLoader::ReadBinormal(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter)
{
	DirectX::XMFLOAT3 result = {};

	if (!(_mesh->GetElementBinormalCount() < 1))
	{
		const FbxGeometryElementBinormal* vBinormal = _mesh->GetElementBinormal(0);
		result = ReadThing<FbxGeometryElementBinormal>(vBinormal, _controlPointIndex, _vertexCounter);
	}
	else
	{
		result = { 0.0f, 0.0f, 0.0f };
	}
	return result;
}

DirectX::XMFLOAT3 FbxLoader::ReadTangent(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter)
{
	DirectX::XMFLOAT3 result = {};

	if (!(_mesh->GetElementTangentCount() < 1))
	{
		const FbxGeometryElementBinormal* vBinormal = _mesh->GetElementBinormal(0);
		result = ReadThing<FbxGeometryElementBinormal>(vBinormal, _controlPointIndex, _vertexCounter);
	}
	else
	{
		result = { 0.0f, 0.0f, 0.0f };
	}
	return result;
}
