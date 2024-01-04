#include "FbxLoader.h"
#include "FbxMetrialData.h"
#include "FbxMeshData.h"

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

	LoadMaterial();

	FbxMeshData* meshData = new FbxMeshData();
	// ������ Ŭ������ ������ �ε�
	LoadMesh(node, meshData);
	data->mesh = meshData;

	return data;
}

void FbxLoader::LoadMesh(FbxNode* _node, FbxMeshData* _data)
{

	// ����� �Ӽ�
	FbxNodeAttribute* nodeAttribute = _node->GetNodeAttribute();
	// ����� ���� ��ȯ ���
	FbxAMatrix& globalTransform = _node->EvaluateGlobalTransform();
	_data->globalTM = FbxAMatrixToXMMatrix(globalTransform);

	// ����� ���� ��ȯ ���
	FbxAMatrix& localTransform = _node->EvaluateLocalTransform();
	_data->localTM = FbxAMatrixToXMMatrix(localTransform);

	// ���� ��ġ ������ ���� ����
	std::vector<DirectX::XMFLOAT3> position;
	// ����ȭ�� ���� ��
	std::unordered_map<VertexF::Data, UINT, VertexF::Data> indexMapping;

	int matCount = _node->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int i = 0; i < matCount; i++)
	{
		FbxSurfaceMaterial* mat = _node->GetSrcObject<FbxSurfaceMaterial>(i);
		if (mat)
		{
			FbxProperty prop = mat->FindProperty(FbxSurfaceMaterial::sDiffuse);
			int layerMatCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
			if (layerMatCount > 0)
			{
				for (int j = 0; j < layerMatCount; j++)
				{
					FbxLayeredTexture* layered_texture = prop.GetSrcObject<FbxLayeredTexture>(j);
					int lcount = layered_texture->GetSrcObjectCount<FbxFileTexture>();
					for (int k = 0; k < lcount; k++)
					{
						FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxFileTexture>(k));
						std::string texture_name = texture->GetRelativeFileName();
						_data->textureFileName.push_back(texture_name);
					}
				}
			}
			else
			{
				int texCount = prop.GetSrcObjectCount<FbxFileTexture>();
				for (int j = 0; j < texCount; j++)
				{
					const FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
					// Then, you can get all the properties of the texture, include its name
					std::string texture_name = texture->GetRelativeFileName();
					_data->textureFileName.push_back(texture_name);
				}
			}
		}
	}
	_data->name = _node->GetName();

	// ��� �Ӽ��� �ִٸ�
	if (nodeAttribute != nullptr)
	{
		// �� ����� �Ӽ��� �޽�.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FbxMesh�� ĳ���õ� ��� �Ӽ��� �����͸� �����´�.
			FbxMesh* mesh = _node->GetMesh();
			int layerCount = mesh->GetLayerCount();
			bool isSame = true;
			std::vector<int> matrialIds;



			// ��ġ ������ �ִ� ���� ������ ������ �´�.
			int vertexCount = mesh->GetControlPointsCount();
			position.resize(vertexCount);

			// �������� ��ġ ������ �Է�
			for (int i = 0; i < vertexCount; i++)
			{
				position[i].x = (static_cast<float>(mesh->GetControlPointAt(i).mData[0]));
				position[i].y = (static_cast<float>(mesh->GetControlPointAt(i).mData[1]));
				position[i].z = (static_cast<float>(mesh->GetControlPointAt(i).mData[2]));
			}

			// �ش� �Ž��� ��� �ﰢ���� ������ �ִ���
			int indexCount = mesh->GetPolygonCount();
			// _data->indexData.resize(static_cast<size_t>(indexCount * 3));
			int vCount = 0;
			// �ﰢ���� ���� ��ŭ vertex ����
			for (int i = 0; i < indexCount; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					int controlPointIndex = mesh->GetPolygonVertex(i, j);

					int matCount = mesh->GetElementMaterialCount();
					DirectX::XMFLOAT3& pos = position[controlPointIndex];
					DirectX::XMFLOAT3 normal = ReadNormal(mesh, controlPointIndex, vCount);
					DirectX::XMFLOAT3 binormal = ReadBinormal(mesh, controlPointIndex, vCount);
					DirectX::XMFLOAT3 tangent = ReadTangent(mesh, controlPointIndex, vCount);

					DirectX::XMFLOAT2 UV = ReadUV(mesh, controlPointIndex, vCount);

					FbxLayerElementMaterial* layerMaterial = mesh->GetLayer(0)->GetMaterials();
					int matrialId = layerMaterial->GetIndexArray().GetAt(i);
					VertexF::Data inputData = VertexF::Data(pos, normal, UV, binormal, tangent, matrialId);

					if (indexMapping.find(inputData) == indexMapping.end())
					{
						UINT temp = static_cast<UINT>(_data->vertexData.size());
						_data->indexData.push_back(temp);
						indexMapping[inputData] = temp;
						_data->vertexData.push_back(inputData);
					}
					else
					{
						_data->indexData.push_back(indexMapping[inputData]);
					}

					vCount++;
				}
			}
		}
	}

	// ����� �ڽĵ� ���� �ε�
	for (int i = 0; i < _node->GetChildCount(); ++i)
	{
		FbxMeshData* child = new FbxMeshData();
		_data->children.push_back(child);
		child->parent = _data;
		LoadMesh(_node->GetChild(i), child);
	}
}

void FbxLoader::LoadSkeleton(FbxNode* _parent, FbxData* _data)
{

}

void FbxLoader::LoadMaterial()
{
	for (int i = 0; i < this->fbxScene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* rawMat = this->fbxScene->GetMaterial(i);
		FbxMetrialData* matData = new FbxMetrialData(i, rawMat);
		metrialList.push_back(matData);
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

DirectX::XMFLOAT2 FbxLoader::ReadUV(const FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter)
{
	DirectX::XMFLOAT2 result = {};

	if (!(_mesh->GetElementUVCount() < 1))
	{
		const FbxGeometryElementUV* vUV = _mesh->GetElementUV(0);

		int counter;
		switch (vUV->GetMappingMode())
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


		switch (vUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vUV->GetDirectArray().GetAt(counter).mData[0]);
			result.y = static_cast<float>(vUV->GetDirectArray().GetAt(counter).mData[1]);
			break;
		}
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vUV->GetIndexArray().GetAt(counter); // �ε����� ���´�.
			result.x = static_cast<float>(vUV->GetDirectArray().GetAt(index).mData[0]);
			result.y = 1 - static_cast<float>(vUV->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		default:
			break;
		}
	}
	else
	{
		result = { 0.0f, 0.0f };
	}
	return result;
}
