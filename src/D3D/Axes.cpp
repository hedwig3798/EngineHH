#include "Axes.h"
#include "DemoCamera.h"
#include "DemoProcess.h"
Axes::Axes(GraphicsEngine* _graphicsEngine, DemoProcess* _scene)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
{

	this->graphicsEngine->CreateInputLayer(&this->pipeline.inputLayout, VertexC::defaultInputLayerDECS, 2, &this->pipeline.vertexShader, path[0]);
	this->graphicsEngine->CreatePixelShader(&this->pipeline.pixelShader, path[1]);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, sizeof(this->vertexes), &this->pipeline.vertexBuffer);
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
	this->graphicsEngine->BindMatrixParameter(
		DirectX::XMMatrixIdentity(),
		this->scene->getCamera()->GetViewTM(),
		this->scene->getCamera()->GetProjectionTM(),
		this->demoMat
	);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}
