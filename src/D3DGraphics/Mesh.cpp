#include "Mesh.h"
#include "ModelParser.h"
#include "GraphicsEngine.h"

Mesh::Mesh()
	: indexList{}
	, pipeline{}
	, indexes{}
	, vertexList{ std::vector<VertexT::Data>() }
	, isLocal(false)
	, vertexes(nullptr)
{
	this->demoMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->demoMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	this->demoMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

Mesh::~Mesh()
{
	delete[] this->indexes;
}

void Mesh::Render(GraphicsEngine* gp, const DirectX::XMMATRIX& _worldTM, const DirectX::XMMATRIX& _viewTM, const DirectX::XMMATRIX& _projTM)
{
	gp->BindMatrixParameter(
		_worldTM,
		_viewTM,
		_projTM,
		this->demoMat
	);
	gp->BindPipeline(this->pipeline);
	gp->SetTexture(0, 1, &this->pipeline.textureView);
	gp->RenderByIndex(this->pipeline, (int)this->indexList.size());
}

void Mesh::CreatePipeline(GraphicsEngine* graphicsEngine, std::wstring _sPath[], std::wstring _texturePath)
{
	// graphicsEngine->CreateTextureData(_texturePath, &this->pipeline.textureView);
	graphicsEngine->CreateInputLayer(this->pipeline, VertexT::defaultInputLayerDECS, _sPath, 5);
	graphicsEngine->CreateVertexBuffer(this->vertexes, static_cast<UINT>(this->vertexList.size()) * VertexT::Size(), &this->pipeline.vertexBuffer);
	graphicsEngine->CreateIndexBuffer(this->indexes, static_cast<UINT>(this->indexList.size()), &this->pipeline.IndexBuffer);
	graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline.vertexStructSize = VertexT::Size();
}

void Mesh::SetVertexesData()
{
	this->vertexList.resize(this->normal.size());
	for (int i = 0; i < (int)this->normal.size(); i++)
	{
		VertexT::Data data;
		data.position = this->position[this->normalIndex[i]];
		data.normal = this->normal[i];

		if (this->textureIndex.size() > 0)
		{
			data.texture = this->texture[this->textureIndex[i]];
		}
		data.weight = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f };
		data.BoneIndices[0] = 0;
		data.BoneIndices[1] = 1;
		data.BoneIndices[2] = 2;
		data.BoneIndices[3] = 3;

		this->vertexList[i] = data;
	}


	int vertexSize = static_cast<int>(this->vertexList.size());
	this->vertexes = new VertexT::Data[vertexSize];
	for (int i = 0; i < vertexSize; i++)
	{
		this->vertexes[i].position = this->vertexList[i].position;
		this->vertexes[i].normal = this->vertexList[i].normal;
		this->vertexes[i].texture = this->vertexList[i].texture;
	}

	// 	this->indexes = new UINT[(int)this->indexList.size()];
	// 	for (int i = 0; i < this->indexList.size(); i++)
	// 	{
	// 		indexes[i] = this->indexList[i];
	// 	}

	this->indexes = new UINT[vertexSize];
	for (int i = 0; i < vertexSize; i += 3)
	{
		if (i + 2 < vertexSize)
		{
			this->indexes[i] = static_cast<UINT>(i);
			this->indexes[i + 1] = static_cast<UINT>(i + 2);
			this->indexes[i + 2] = static_cast<UINT>(i + 1);
		}
		else
		{
			assert(false);
		}
	}
}
