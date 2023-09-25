#include "DemoProcess.h"
#include "GraphicsEngine.h"
#include "DemoCamera.h"
#include "DemoObject.h"

DemoProcess::DemoProcess()
	: graphicsEngine(nullptr)
	, hwnd(nullptr)
	, camera(nullptr)
	, object(nullptr)
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

	RECT windowSize;
	GetWindowRect(this->hwnd, &windowSize);

	this->camera = new DemoCamera((float)(windowSize.bottom - windowSize.top), (float)(windowSize.right - windowSize.left));
	this->object = new DemoObject(this->graphicsEngine, this);
}

void DemoProcess::Process()
{
	Update();
	Render();
}

DemoCamera* DemoProcess::getCamera()
{
	return this->camera;
}

void DemoProcess::Update()
{
	camera->Update();
}

void DemoProcess::Render()
{
	this->graphicsEngine->begineDraw();
	this->object->Render(graphicsEngine);
	this->graphicsEngine->endDraw();
}
