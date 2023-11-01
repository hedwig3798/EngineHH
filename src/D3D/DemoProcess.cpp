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
	, axes(nullptr)
{
	this->explain = L"W, S : ī�޶� ����, �Ĺ� �̵�\n";
	this->explain += L"A, D : ī�޶� �¿� �̵�\n";
	this->explain += L"Q, E : ī�޶� ���� �̵�\n";
	this->explain += L"ȭ��ǥ : ī�޶� ȸ��\n";
	this->explain += L"���� 0, 1, 2, 3 : ���� ���� ����\n";
	this->explain += L"���� 4: ��� ��带 ���� ��ġ��\n";
	this->explain += L"���� 5: ��� ��带 ���� ��ġ��\n";
	
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


	this->line = new LineObject(this->graphicsEngine, this);
	this->axes = new Axes(this->graphicsEngine, this);

	this->managers = new ManagerSet();
	this->managers->Initialize(this->hwnd);
	this->object = new DemoObject(this->graphicsEngine, this, this->managers);
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
	this->object->Update(this->managers->timeManager->GetfDT());
}

void DemoProcess::Render()
{
	this->graphicsEngine->begineDraw();

	this->object->Render(graphicsEngine);
	this->line->Render(graphicsEngine);
	this->axes->Render(graphicsEngine);
	std::wstring dt = L"DeltaTime : ";
	dt += std::to_wstring(this->managers->timeManager->GetfDT());
	dt += L"\n";
	dt += L"FPS : ";
	dt += std::to_wstring(1 / this->managers->timeManager->GetfDT());


	this->graphicsEngine->WriteText(10, 12, COLORS::White, const_cast<TCHAR*>(this->explain.c_str()));
	this->graphicsEngine->WriteText(200, 12, COLORS::White, const_cast<TCHAR*>(dt.c_str()));

	this->graphicsEngine->endDraw();
}
