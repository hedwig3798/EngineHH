#include "LineObject.h"

LineObject::LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, VertexC::Vertex _start, VertexC::Vertex _end)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, vertexes{_start, _end}
{
	this->graphicsEngine->CreateInputLayer(&this->pipeline.inputLayout, &this->pipeline.vertexShader, &this->pipeline.pixelShader);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, (UINT)sizeof(this->vertexes) * VertexC::Size(), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(indexes, sizeof(indexes), &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

LineObject::~LineObject()
{
	this->pipeline.relasePipline();
}

void LineObject::Update(float _dt)
{

}

void LineObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}
