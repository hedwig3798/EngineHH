#include "DemoObject.h"
#include "DemoProcess.h"
#include "DemoCamera.h"
#include "ModelParser.h"
DemoObject::DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
{
	std::vector<VertexT::Data> vertexInfo;
	std::vector<UINT> indexInfo;
	TestParser(vertexInfo, indexInfo, L"../Model/testPolygon.txt");

	this->vertexesSize = (int)vertexInfo.size();
	this->indexesSize = (int)indexInfo.size();

	this->vertexes = new VertexT::Data[this->vertexesSize];
	for (int i = 0; i < vertexesSize; i++)
	{
		vertexes[i] = vertexInfo[i];
	}

	this->indexes = new UINT[this->indexesSize];
	for (int i = 0; i < indexesSize; i++)
	{
		indexes[i] = indexInfo[i];
	}
	this->graphicsEngine->CreateTextureData(this->texturePath, &this->pipeline.textureView);
	this->graphicsEngine->CreateInputLayer(this->pipeline, VertexT::defaultInputLayerDECS, this->path, 3);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, this->vertexesSize * VertexT::Size(), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(this->indexes, this->indexesSize, &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	this->pipeline.vertexStructSize = VertexT::Size();
}

DemoObject::~DemoObject()
{
	this->pipeline.RelasePipline();
	delete[] vertexes;
	delete[] indexes;
}

void DemoObject::Update(float _dt)
{

}

void DemoObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->SetTexture(0, 1, &this->pipeline.textureView);
	this->graphicsEngine->RenderByIndex(pipeline, this->indexesSize);
}
