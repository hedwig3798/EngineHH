#include "DemoObject.h"
#include "DemoProcess.h"
#include "DemoCamera.h"

DemoObject::DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
{
	this->graphicsEngine->CreateInputLayer(&this->pipeline.inputLayout, &this->pipeline.vertexShader, &this->pipeline.pixelShader);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, sizeof(this->vertexes), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(indexes, sizeof(indexes), &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);
	this->pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

DemoObject::~DemoObject()
{

}

void DemoObject::Update(float _dt)
{

}

void DemoObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindPipeline(pipeline);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}
