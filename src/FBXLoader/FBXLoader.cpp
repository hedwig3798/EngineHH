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
	this->fbxManager = FbxManager::Create();
	assert(this->fbxManager == nullptr && "cannot create fbx manager\n");

	this->fbxImpoter = FbxImporter::Create(fbxManager, "");
	assert(this->fbxManager == nullptr && "cannot create fbx manager\n");

	this->fbxScene = FbxScene::Create(fbxManager, "");
	assert(this->fbxScene == nullptr && "cannot create fbx manager\n");

	this->geometryConverter = new FbxGeometryConverter(this->fbxManager);
}

void FbxLoader::Release()
{
	this->fbxManager->Destroy();
	this->fbxImpoter->Destroy();
	this->fbxScene->Destroy();
	delete this->geometryConverter;
}

FbxData* FbxLoader::Load(std::string _path)
{
	bool ret = this->fbxImpoter->Initialize(_path.c_str());
	assert(ret && "cannot initalize fbx importer when load\n");

	ret = this->fbxImpoter->Import(this->fbxScene);
	assert(ret && "cannot import fbx scene when load\n");

	FbxNode* node = this->fbxScene->GetRootNode();
	assert(node && "cannot load Node from scene\n");

	this->geometryConverter->Triangulate(this->fbxScene, true);

	FbxData* data = new FbxData();

	LoadNode(node, data);

	return data;
}

void FbxLoader::LoadNode(FbxNode* _parent, FbxData* _data)
{
	FbxNodeAttribute* nodeAttribute = _parent->GetNodeAttribute();

	// �� ����� �Ӽ��� �޽�.
	if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		// FbxMesh�� ĳ���õ� ��� �Ӽ��� �����͸� �����´�.
		FbxMesh* mesh = _parent->GetMesh();

		int vertexCount = mesh->GetControlPointsCount();
		_data->ResizeVertex(vertexCount);
		for (int i = 0; i < vertexCount; i++)
		{
			_data->vertexData[i].position.x = (static_cast<float>(mesh->GetControlPointAt(i).mData[0]));
			_data->vertexData[i].position.x = (static_cast<float>(mesh->GetControlPointAt(i).mData[1]));
			_data->vertexData[i].position.x = (static_cast<float>(mesh->GetControlPointAt(i).mData[2]));
		}

		int indexCount = mesh->GetPolygonCount();
		_data->ResizeIndex(indexCount);
		for (int i = 0; i < indexCount; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				_data->indexData[i] = mesh->GetPolygonVertex(i, j);
			}
		}
	}

	for (int i = 0; i < _parent->GetChildCount(); ++i)
	{
		FbxData* child = new FbxData();
		_data->children.push_back(child);
		child->parent = _data;
		LoadNode(_parent->GetChild(i), child);
	}
}
