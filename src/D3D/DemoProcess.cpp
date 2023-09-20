#include "DemoProcess.h"
#include "GraphicsEngine.h"

DemoProcess::DemoProcess()
	: graphicsEngine(nullptr)
	, hwnd(nullptr)
	, pipeline{}
{

}

DemoProcess::~DemoProcess()
{

}

void DemoProcess::Initialize(HWND _hwnd)
{
	this->hwnd = _hwnd;
	this->graphicsEngine = new GraphicsEngine();
	this->graphicsEngine->Initialize(this->hwnd);
	this->graphicsEngine->RenderClearView();


	this->graphicsEngine->CreateInputLayer(this->pipeline.inputLayout, this->pipeline.vertexShader, this->pipeline.pixelShader);
	this->graphicsEngine->SetParameter(DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity());
	this->graphicsEngine->CreateVertexBuffer(this->tempVertex, sizeof(this->tempVertex), this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(tempIndex, sizeof(tempIndex), this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(this->pipeline.rasterizerState);
}

void DemoProcess::Process()
{
	Update();
	Render();
}

void DemoProcess::Update()
{

}

void DemoProcess::Render()
{
	this->graphicsEngine->begineDraw();
	this->graphicsEngine->RenderTestThing(this->pipeline);
	this->graphicsEngine->endDraw();
}
