#include "Mesh.h"
#include "ModelParser.h"
#include "GraphicsEngine.h"

Mesh::Mesh()
	: indexList{}
	, pipeline{}
	, vertexes{}
	, indexes{}
{
}

Mesh::~Mesh()
{
}

void Mesh::Render(GraphicsEngine* gp)
{
	gp->BindPipeline(this->pipeline);
	gp->SetTexture(0, 1, &this->pipeline.textureView);
	gp->RenderByIndex(this->pipeline, (int)this->indexList.size());
}

void Mesh::CreatePipeline(GraphicsEngine* graphicsEngine, std::wstring _sPath[], std::wstring _texturePath)
{
	this->vertexes = new VertexT::Data[(int)this->vertexList.size()];
	for (int i = 0; i < this->vertexList.size(); i++)
	{
		vertexes[i] = this->vertexList[i];
	}

// 	this->indexes = new UINT[(int)this->indexList.size()];
// 	for (int i = 0; i < this->indexList.size(); i++)
// 	{
// 		indexes[i] = this->indexList[i];
// 	}

	this->indexes = new UINT[(UINT)this->vertexList.size()];
	for (size_t i = 0; i < this->vertexList.size(); i = i + 3)
	{
		this->indexes[i] = (UINT)i;
		this->indexes[i + 1] = (UINT)(i + 2);
		this->indexes[i + 2] = (UINT)(i + 1);
	}

	graphicsEngine->CreateTextureData(_texturePath, &this->pipeline.textureView);
	graphicsEngine->CreateInputLayer(this->pipeline, VertexT::defaultInputLayerDECS, _sPath, 3);
	graphicsEngine->CreateVertexBuffer(this->vertexes, this->vertexList.size() * VertexT::Size(), &this->pipeline.vertexBuffer);
	graphicsEngine->CreateIndexBuffer(this->indexes, this->indexList.size(), &this->pipeline.IndexBuffer);
	graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline.vertexStructSize = VertexT::Size();
}
