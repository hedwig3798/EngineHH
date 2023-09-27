#include "DemoProcess.h"
#include "GraphicsEngine.h"
#include "DemoCamera.h"
#include "DemoObject.h"
#include "ManagerSet.h"
#include "LineObject.h"

DemoProcess::DemoProcess()
	: graphicsEngine(nullptr)
	, hwnd(nullptr)
	, camera(nullptr)
	, object(nullptr)
	, managers(nullptr)
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


	this->object = new DemoObject(this->graphicsEngine, this);

	this->managers = new ManagerSet();
	this->managers->Initialize(this->hwnd);
	this->camera = new DemoCamera((float)(windowSize.bottom - windowSize.top), (float)(windowSize.right - windowSize.left), this->managers);

	for (size_t i = 1; i < 5; i++)
	{
		Vertex start = { DirectX::XMFLOAT3{-5.0f, 0, (float)i}, COLORS::White };
		Vertex end = { DirectX::XMFLOAT3{5.0f, 0, (float)i}, COLORS::White };
		lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

		start = { DirectX::XMFLOAT3{-5.0f, 0, -(float)i}, COLORS::White };
		end = { DirectX::XMFLOAT3{5.0f, 0, -(float)i}, COLORS::White };
		lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

		start = { DirectX::XMFLOAT3{(float)i, 0, -5.0f}, COLORS::White };
		end = { DirectX::XMFLOAT3{(float)i, 0, 5.0f}, COLORS::White };
		lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

		start = { DirectX::XMFLOAT3{-(float)i, 0, -5.0f}, COLORS::White };
		end = { DirectX::XMFLOAT3{-(float)i, 0, 5.0f}, COLORS::White };
		lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));
	}
	Vertex start = { DirectX::XMFLOAT3{0, 0, 0}, COLORS::Blue };
	Vertex end = { DirectX::XMFLOAT3{0, 0, 10}, COLORS::Blue };
	lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));
	start = { DirectX::XMFLOAT3{0, 0, 0}, COLORS::White };
	end = { DirectX::XMFLOAT3{0, 0, -5}, COLORS::White };
	lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

	start = { DirectX::XMFLOAT3{0, 0, 0}, COLORS::Red };
	end = { DirectX::XMFLOAT3{0, 10.0f, 0}, COLORS::Red };
	lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

	start = { DirectX::XMFLOAT3{0, 0, 0}, COLORS::Green };
	end = { DirectX::XMFLOAT3{10, 0, 0}, COLORS::Green };
	lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

	start = { DirectX::XMFLOAT3{0, 0, 0}, COLORS::White };
	end = { DirectX::XMFLOAT3{-5, 0, 0}, COLORS::White };
	lineList.push_back(new LineObject(this->graphicsEngine, this, start, end));

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
	for (SIZE_T i = 0; i < this->lineList.size(); i++)
	{
		lineList[i]->Render(graphicsEngine);
	}
	TCHAR* explain = (TCHAR*)(L"W, S : 카메라 전방, 후방 이동\nA, D : 카메라 좌우 이동\n화살표 : 카메라 회전");
	this->graphicsEngine->WriteText(10, 10, COLORS::White, explain);

	this->graphicsEngine->endDraw();
}
