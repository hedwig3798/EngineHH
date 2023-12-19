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
	// Fbx �ε带 ���� Ŭ���� ����
	this->fbxManager = FbxManager::Create();
	assert(this->fbxManager && "cannot create fbx manager\n");

	this->fbxImpoter = FbxImporter::Create(fbxManager, "");
	assert(this->fbxManager && "cannot create fbx manager\n");

	this->fbxScene = FbxScene::Create(fbxManager, "");
	assert(this->fbxScene && "cannot create fbx manager\n");

	// �Ž��� �ﰢ������ �߶��� Ŭ���� ����
	this->geometryConverter = new FbxGeometryConverter(this->fbxManager);
}

void FbxLoader::Release()
{
	// �̸� ������ Ŭ���� ����
	this->fbxManager->Destroy();
	this->fbxImpoter->Destroy();
	this->fbxScene->Destroy();
	delete this->geometryConverter;
}

FbxData* FbxLoader::Load(std::string _path)
{
	// ��θ� ���� ������ Ŭ���� �ʱ�ȭ
	bool ret = this->fbxImpoter->Initialize(_path.c_str());
	assert(ret && "cannot initalize fbx importer when load\n");

	// �����͸� ���� �� ����Ʈ
	ret = this->fbxImpoter->Import(this->fbxScene);
	assert(ret && "cannot import fbx scene when load\n");

	// ������ ��Ʈ ��� ������
	FbxNode* node = this->fbxScene->GetRootNode();
	assert(node && "cannot load Node from scene\n");

	// �ش� ���� ��带 �ﰢ������ �ɰ���
	this->geometryConverter->Triangulate(this->fbxScene, true);

	// ������ ������ Ŭ����
	FbxData* data = new FbxData();
	// ������ Ŭ������ ������ �ε�
	LoadNode(node, data);

	return data;
}

void FbxLoader::LoadNode(FbxNode* _node, FbxData* _data)
{
	// ����� �Ӽ�
	FbxNodeAttribute* nodeAttribute = _node->GetNodeAttribute();

	// ����� ���� ��ȯ ���
	FbxAMatrix& globalTransform = _node->EvaluateGlobalTransform();
	_data->globalTM = FbxAMatrixToXMMatrix(globalTransform);

	// ����� ���� ��ȯ ���
	FbxAMatrix& localTransform = _node->EvaluateLocalTransform();
	_data->localTM = FbxAMatrixToXMMatrix(localTransform);

	// ��� �Ӽ��� �ִٸ�
	if (nodeAttribute != nullptr)
	{
		// �� ����� �Ӽ��� �޽�.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FbxMesh�� ĳ���õ� ��� �Ӽ��� �����͸� �����´�.
			fbxsdk::FbxMesh* mesh = _node->GetMesh();

			// ��ġ ������ �ִ� ���� ������ ������ �´�.
			int vertexCount = mesh->GetControlPointsCount();
			_data->position.resize(vertexCount);

			// �������� ��ġ ������ �Է�
			for (int i = 0; i < vertexCount; i++)
			{
				_data->position[i].x = (static_cast<float>(mesh->GetControlPointAt(i).mData[0]));
				_data->position[i].y = (static_cast<float>(mesh->GetControlPointAt(i).mData[1]));
				_data->position[i].z = (static_cast<float>(mesh->GetControlPointAt(i).mData[2]));
			}

			// �ش� �Ž��� ��� �ﰢ���� ������ �ִ���
			int indexCount = mesh->GetPolygonCount();
			_data->indexData.resize(static_cast<size_t>(indexCount * 3));

			// �ﰢ���� ���� ��ŭ vertex ����
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

	// ����� �ڽĵ� ���� �ε�
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
	// fbx�� ��� �����͸� Diretx�� �µ��� ����
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
