#include "Axes.h"
Axes::Axes(GraphicsEngine* _graphicsEngine, DemoProcess* _scene)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
{

	this->graphicsEngine->CreateInputLayer(this->pipeline, VertexC::defaultInputLayerDECS, this->path, 2);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, sizeof(this->vertexes) * VertexC::Size(), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(indexes, sizeof(indexes), &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	this->pipeline.vertexStructSize = VertexC::Size();
}

Axes::~Axes()
{
	this->pipeline.RelasePipline();
}

void Axes::Update(float _dt)
{

}

void Axes::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}
