#include "DemoProcess.h"
#include "GraphicsEngine.h"

DemoProcess::DemoProcess()
{

}

DemoProcess::~DemoProcess()
{

}

void DemoProcess::Initialize(HWND _hwnd)
{
	this->hwnd = _hwnd;
	graphicsEngine = new GraphicsEngine();
	graphicsEngine->Initialize(this->hwnd);
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
	this->graphicsEngine->RenderClearView();
}
