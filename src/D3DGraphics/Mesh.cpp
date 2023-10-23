#include "Mesh.h"
#include "ModelParser.h"
#include "GraphicsEngine.h"

Mesh::Mesh()
	: origianlVertexList{}
	, indexList{}
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
	gp->RenderByIndex(this->pipeline, (int)this->indexList.size());
}

void Mesh::CreatePipeline(GraphicsEngine* graphicsEngine, std::wstring _sPath[], std::wstring _texturePath)
{
	this->vertexes = new VertexT::Data[(int)this->optimizeVertexList.size()];
	for (int i = 0; i < this->optimizeVertexList.size(); i++)
	{
		vertexes[i] = this->optimizeVertexList[i];
	}

// 	this->indexes = new UINT[(int)this->indexList.size()];
// 	for (int i = 0; i < this->indexList.size(); i++)
// 	{
// 		indexes[i] = this->indexList[i];
// 	}

	this->indexes = new UINT[(UINT)this->optimizeVertexList.size()];
	for (size_t i = 0; i < this->optimizeVertexList.size(); i = i + 3)
	{
		indexes[i] = (UINT)i;
		indexes[i + 1] = (UINT)(i + 2);
		indexes[i + 2] = (UINT)(i + 1);
	}

	graphicsEngine->CreateTextureData(_texturePath, &this->pipeline.textureView);
	graphicsEngine->CreateInputLayer(this->pipeline, VertexT::defaultInputLayerDECS, _sPath, 3);
	graphicsEngine->CreateVertexBuffer(this->vertexes, this->optimizeVertexList.size() * VertexT::Size(), &this->pipeline.vertexBuffer);
	graphicsEngine->CreateIndexBuffer(this->indexes, this->indexList.size(), &this->pipeline.IndexBuffer);
	graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline.vertexStructSize = VertexT::Size();
}
