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

	DirectX::XMMATRIX w = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX v = DirectX::XMMatrixIdentity();
	v *= DirectX::XMMatrixLookAtLH(DirectX::XMVECTOR{ 2, 2, 2 }, DirectX::XMVECTOR{ 0, 0, 0 }, DirectX::XMVECTOR{ 0, 1, 0 });
	// v *= DirectX::XMMatrixTranslation(2.0f, 2.0f, 2.0f);
	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(1.7f, 800.0f/600.0f, 1, 1000);

	this->graphicsEngine->CreateInputLayer(&this->pipeline.inputLayout, &this->pipeline.vertexShader, &this->pipeline.pixelShader);
	this->graphicsEngine->SetParameter(w, v, p);
	this->graphicsEngine->CreateVertexBuffer(this->tempVertex, sizeof(this->tempVertex), &this->pipeline.vertexBuffer);
	this->graphicsEngine->CreateIndexBuffer(tempIndex, sizeof(tempIndex), &this->pipeline.IndexBuffer);
	this->graphicsEngine->CreateRasterizerState(&this->pipeline.rasterizerState);

	this->graphicsEngine->BindPipeline(pipeline);
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
