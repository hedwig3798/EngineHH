#include "DemoObject.h"
#include "DemoProcess.h"
#include "DemoCamera.h"
#include "ModelParser.h"
DemoObject::DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
{
	std::vector<VertexC::Vertex> vertexInfo;
	std::vector<UINT> indexInfo;
	GetVertexAndIndex(vertexInfo, indexInfo, L"../Model/Polygon.obj");

	this->vertexesSize = (int)vertexInfo.size();
	this->indexesSize = (int)indexInfo.size();

	this->vertexes = new VertexC::Vertex[this->vertexesSize];
	for (int i = 0; i < vertexesSize; i++)
	{
		vertexes[i] = vertexInfo[i];
	}

	this->indexes = new UINT[this->indexesSize];
	for (int i = 0; i < indexesSize; i++)
	{
		indexes[i] = indexInfo[i] - 1;
	}

	this->graphicsEngine->CreateInputLayer(&this->pipeline.inputLayout, &this->pipeline.vertexShader, &this->pipeline.pixelShader);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, this->vertexesSize * VertexC::Size(), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(this->indexes, this->indexesSize, &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

}

DemoObject::~DemoObject()
{
	this->pipeline.relasePipline();
	delete[] vertexes;
	delete[] indexes;
}

void DemoObject::Update(float _dt)
{

}

void DemoObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->RenderByIndex(pipeline, this->indexesSize);
}
