#include "LineObject.h"

LineObject::LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, Vertex _start, Vertex _end) 
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, vertexes{_start, _end}
{
	this->graphicsEngine->CreateInputLayer(&this->pipeline.inputLayout, &this->pipeline.vertexShader, &this->pipeline.pixelShader);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, sizeof(this->vertexes), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(indexes, sizeof(indexes), &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

LineObject::~LineObject()
{

}

void LineObject::Update(float _dt)
{

}

void LineObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}
