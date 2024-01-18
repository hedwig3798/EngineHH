#include "DemoProcess.h"
#include "IGraphicsEngine.h"
#include "DemoObject.h"
#include "ManagerSet.h"
#include "ICamera.h"

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
	this->explain += L"마우스 좌클릭 드래그: 카메라 회전\n";
	staticManagers = this->managers;
}

DemoProcess::~DemoProcess()
{
	delete this->graphicsEngine;
	delete this->object;
	delete this->camera;
	delete this->managers;
}

void DemoProcess::Initialize(HWND _hwnd)
{
	this->hwnd = _hwnd;
	CreateGrapicsEngine(&graphicsEngine);
	this->graphicsEngine->Initialize(this->hwnd);

	RECT windowSize;
	GetWindowRect(this->hwnd, &windowSize);

	this->graphicsEngine->CreateCamera(
		&this->camera,
		static_cast<float>(windowSize.bottom - windowSize.top),
		static_cast<float>(windowSize.right - windowSize.left));
	this->graphicsEngine->SetMainCamera(this->camera);

// 	this->line = new LineObject(this->graphicsEngine, this);
// 	this->axes = new Axes(this->graphicsEngine, this);

	this->managers = new ManagerSet();
	this->staticManagers = this->managers;
	this->managers->Initialize(this->hwnd);
	this->object = new DemoObject(this->graphicsEngine, this, this->managers);
}

void DemoProcess::Process()
{
	Update();
	Render();
}

void DemoProcess::Update()
{
	this->managers->Update();
	this->object->Update(this->managers->timeManager->GetfDT());
	CameraUpdate(this->managers->timeManager->GetfDT());
	this->managers->keyManager->mouseDX = 0;
	this->managers->keyManager->mouseDY = 0;
}

void DemoProcess::Render()
{
	this->graphicsEngine->begineDraw();

	this->object->Render(graphicsEngine);
	
	this->graphicsEngine->DrawDefaultAxes();
	this->graphicsEngine->DrawDefaultLine();

	std::wstring dt = L"DeltaTime : ";
	dt += std::to_wstring(this->managers->timeManager->GetfDT());
	dt += L"\n";
	dt += L"FPS : ";
	dt += std::to_wstring(1 / this->managers->timeManager->GetfDT());

	float w[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->graphicsEngine->WriteText(10, 12, w, const_cast<TCHAR*>(this->explain.c_str()));
	this->graphicsEngine->WriteText(200, 12, w, const_cast<TCHAR*>(dt.c_str()));

	this->graphicsEngine->endDraw();
	int test = 0;
}

void DemoProcess::CameraUpdate(float _dt)
{
	float speed = 100;
	if (this->managers->keyManager->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		this->camera->RotateRight(this->managers->timeManager->GetfDT());
	}
	if (this->managers->keyManager->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		this->camera->RotateRight(-this->managers->timeManager->GetfDT());
	}
	if (this->managers->keyManager->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		this->camera->RotateUp(-this->managers->timeManager->GetfDT());
	}
	if (this->managers->keyManager->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
	{
		this->camera->RotateUp(this->managers->timeManager->GetfDT());
	}

	if (this->managers->keyManager->GetKeyState(KEY::W) == KEY_STATE::HOLD)
	{
		this->camera->MoveFoward(this->managers->timeManager->GetfDT() * speed);
	}
	if (this->managers->keyManager->GetKeyState(KEY::S) == KEY_STATE::HOLD)
	{
		this->camera->MoveFoward(-this->managers->timeManager->GetfDT() * speed);
	}
	if (this->managers->keyManager->GetKeyState(KEY::A) == KEY_STATE::HOLD)
	{
		this->camera->MoveRight(-this->managers->timeManager->GetfDT() * speed);
	}
	if (this->managers->keyManager->GetKeyState(KEY::D) == KEY_STATE::HOLD)
	{
		this->camera->MoveRight(this->managers->timeManager->GetfDT() * speed);
	}
	if (this->managers->keyManager->GetKeyState(KEY::Q) == KEY_STATE::HOLD)
	{
		this->camera->MoveUP(-this->managers->timeManager->GetfDT() * speed);
	}
	if (this->managers->keyManager->GetKeyState(KEY::E) == KEY_STATE::HOLD)
	{
		this->camera->MoveUP(this->managers->timeManager->GetfDT() * speed);
	}

	if (this->managers->keyManager->GetMouseState(MOUSE::LEFT) == KEY_STATE::HOLD)
	{
		this->camera->RotateRight(this->managers->keyManager->mouseDX * 0.003f);
		this->camera->RotateUp(this->managers->keyManager->mouseDY * 0.003f);
	}

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
		staticManagers->keyManager->OnMouseMove(static_cast<int>(wParam), LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}