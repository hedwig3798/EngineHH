#include "DemoProcess.h"
#include "GraphicsEngine.h"
#include "DemoCamera.h"
#include "DemoObject.h"
#include "ManagerSet.h"
#include "LineObject.h"
#include "Axes.h"

ManagerSet* DemoProcess::staticManagers = nullptr;

DemoProcess::DemoProcess()
	: graphicsEngine(nullptr)
	, hwnd(nullptr)
	, camera(nullptr)
	, object(nullptr)
	, managers(nullptr)
	, line(nullptr)
	, axes(nullptr)
{
	this->explain = L"W, S : 카메라 전방, 후방 이동\n";
	this->explain += L"A, D : 카메라 좌우 이동\n";
	this->explain += L"Q, E : 카메라 상하 이동\n";
	this->explain += L"화살표 : 카메라 회전\n";
	this->explain += L"숫자 0, 1, 2, 3 : 조명 갯수 설정\n";
	this->explain += L"숫자 4: 모든 노드를 로컬 위치로\n";
	this->explain += L"숫자 5: 모든 노드를 월드 위치로\n";
	staticManagers = this->managers;
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
	this->staticManagers = this->managers;
	this->managers->Initialize(this->hwnd);
	this->object = new DemoObject(this->graphicsEngine, this, this->managers);
	this->camera = new DemoCamera(
		static_cast<float>(windowSize.bottom - windowSize.top), 
		static_cast<float>(windowSize.right - windowSize.left),
		this->managers);
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
	int test = 0;
}

LRESULT CALLBACK DemoProcess::WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		staticManagers->keyManager->OnMouseLeftDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		staticManagers->keyManager->OnMouseRightDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		staticManagers->keyManager->OnMouseLeftUp(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		staticManagers->keyManager->OnMouseRightUp(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		staticManagers->keyManager->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}