#include "LineObject.h"

LineObject::LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, VertexC::Data _start, VertexC::Data _end)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, vertexes{_start, _end}
{
	this->graphicsEngine->CreateInputLayer(this->pipeline, VertexC::defaultInputLayerDECS, this->path, 2);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, 2 * VertexC::Size(), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(indexes, sizeof(indexes), &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	this->pipeline.vertexStructSize = VertexC::Size();
}

LineObject::~LineObject()
{
	this->pipeline.RelasePipline();
}

void LineObject::Update(float _dt)
{

}

void LineObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}