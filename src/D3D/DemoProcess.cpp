#include "DemoProcess.h"
#include "GraphicsEngine.h"
#include "DemoCamera.h"
#include "DemoObject.h"
#include "ManagerSet.h"
#include "LineObject.h"
#include "Axes.h"

DemoProcess::DemoProcess()
	: graphicsEngine(nullptr)
	, hwnd(nullptr)
	, camera(nullptr)
	, object(nullptr)
	, managers(nullptr)
	, line(nullptr)
{
}

DemoProcess::~DemoProcess()
{
	delete this->graphicsEngine;
	delete this->object;

	delete this->line;
	
	delete this->camera;
	delete this->managers;
}

void DemoProcess::Initialize(HWND _hwnd)
{
	this->hwnd = _hwnd;
	this->graphicsEngine = new GraphicsEngine();
	this->graphicsEngine->Initialize(this->hwnd);
	this->graphicsEngine->RenderClearView();

	RECT windowSize;
	GetWindowRect(this->hwnd, &windowSize);


	this->object = new DemoObject(this->graphicsEngine, this);
	this->line = new LineObject(this->graphicsEngine, this);
	this->axes = new Axes(this->graphicsEngine, this);

	this->managers = new ManagerSet();
	this->managers->Initialize(this->hwnd);
	this->camera = new DemoCamera((float)(windowSize.bottom - windowSize.top), (float)(windowSize.right - windowSize.left), this->managers);
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
	this->managers->Update();
	camera->Update();
}

void DemoProcess::Render()
{
	this->graphicsEngine->begineDraw();

	this->graphicsEngine->SetParameter(
		DirectX::XMMatrixIdentity(),
		this->camera->GetViewTM(),
		this->camera->GetProjectionTM()
	);

	this->object->Render(graphicsEngine);
	this->line->Render(graphicsEngine);
	this->axes->Render(graphicsEngine);
	TCHAR* explain = (TCHAR*)(L"W, S : 카메라 전방, 후방 이동\nA, D : 카메라 좌우 이동\n화살표 : 카메라 회전");
	this->graphicsEngine->WriteText(10, 10, COLORS::White, explain);

	this->graphicsEngine->endDraw();
}
