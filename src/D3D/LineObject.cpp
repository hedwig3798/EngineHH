#include "LineObject.h"
#include "DemoProcess.h"
#include "DemoCamera.h"
LineObject::LineObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
{
	for (int i = 0; i < 100; i++)
	{
		vertexes[i].position = DirectX::XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertexes[i].color = COLORS::White;
	}

	for (int i = 0; i < 10; i++)
	{
		indexes[i * 2] = i;
		indexes[i * 2 + 1] = i + 90;
	}

	for (int i = 0; i < 10; i++)
	{
		indexes[20 + (i * 2)] = i * 10;
		indexes[20 + (i * 2) + 1] = i * 10 + 9;
	}

	this->graphicsEngine->CreateInputLayer(this->pipeline, VertexC::defaultInputLayerDECS, this->path, 2);
	this->graphicsEngine->CreateVertexBuffer(this->vertexes, sizeof(this->vertexes), &this->pipeline.vertexBuffer);
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
	this->graphicsEngine->BindMatrixParameter(
		DirectX::XMMatrixIdentity(),
		this->scene->getCamera()->GetViewTM(),
		this->scene->getCamera()->GetProjectionTM(),
		this->demoMat
	);
	this->graphicsEngine->RenderByIndex(pipeline, sizeof(this->indexes) / sizeof(this->indexes[0]));
}
