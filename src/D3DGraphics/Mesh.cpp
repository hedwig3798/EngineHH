#include "Mesh.h"
#include "ModelParser.h"
#include "GraphicsEngine.h"

Mesh::Mesh()
	: indexList{}
	, pipeline{}
	, vertexes{}
	, indexes{}
	, vertexList{ std::vector<VertexT::Data>() }

{
}

Mesh::~Mesh()
{
	delete[] this->vertexes;
	delete[] this->indexes;
}

void Mesh::Render(GraphicsEngine* gp)
{
	ChangeVertex(gp);
	gp->BindPipeline(this->pipeline);
	gp->SetTexture(0, 1, &this->pipeline.textureView);
	gp->RenderByIndex(this->pipeline, (int)this->indexList.size());
}

void Mesh::CreatePipeline(GraphicsEngine* graphicsEngine, std::wstring _sPath[], std::wstring _texturePath)
{
	int vertexSize = static_cast<int>(this->vertexList.size());
	this->vertexes = new VertexT::Data[vertexSize];
	for (int i = 0; i < vertexSize; i++)
	{
		this->vertexes[i] = this->vertexList[i];
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
	}

	// graphicsEngine->CreateTextureData(_texturePath, &this->pipeline.textureView);
	graphicsEngine->CreateInputLayer(this->pipeline, VertexT::defaultInputLayerDECS, _sPath, 3);
	graphicsEngine->CreateVertexBuffer(this->vertexes, (UINT)this->vertexList.size() * VertexT::Size(), &this->pipeline.vertexBuffer);
	graphicsEngine->CreateIndexBuffer(this->indexes, (UINT)this->indexList.size(), &this->pipeline.IndexBuffer);
	graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline.vertexStructSize = VertexT::Size();
}

void Mesh::ChangeVertex(GraphicsEngine* gp)
{
	this->pipeline.vertexBuffer->Release();
	gp->CreateVertexBuffer(this->vertexes, (UINT)this->vertexList.size() * VertexT::Size(), &this->pipeline.vertexBuffer);
}
