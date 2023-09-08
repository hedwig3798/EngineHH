#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
	: featureLevel{}
	, d3d11Device(nullptr)
	, d3d11DeviceContext(nullptr)
	, hwnd(nullptr)
	, swapChain(nullptr)
	, renderTargetView(nullptr)
	, m4xMsaaQuality(-1)
	, depthStancilBuffer(nullptr)
	, depthStancilView(nullptr)
{
}

GraphicsEngine::~GraphicsEngine()
{
}

/// <summary>
/// �׷��� ���� �ʱ�ȭ
/// </summary>
/// <param name="_hwnd">������ �ڵ�</param>
void GraphicsEngine::Initialize(HWND _hwnd)
{
	this->hwnd = _hwnd;

	CreateD3D11DeviceContext();
	CreateChainValue();
	CreateRenderTargetView();
	CreateDepthStencilBufferAndView();
}

/// <summary>
/// ȭ���� Ư�� ������ �ʱ�ȭ
/// </summary>
void GraphicsEngine::RenderClearView()
{
	ClearRenderTargetView();
	ClearDepthStencilView();
	this->swapChain->Present(0, 0);
}

/// <summary>
/// D3D11 ����̽��� ����̽� ���ؽ�Ʈ ����
/// </summary>
void GraphicsEngine::CreateD3D11DeviceContext()
{
	// D3D11 ����̽� ����
	HRESULT hr;
	hr = D3D11CreateDevice(
		0, // �⺻ ���
		D3D_DRIVER_TYPE_HARDWARE,
		0, // ����Ʈ���� ��ġ ��� ���� ����
		this->createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION, // D3D11 ����
		&this->d3d11Device,
		&this->featureLevel,
		&this->d3d11DeviceContext
	);
	assert(hr == S_OK && "cannot create d3d11 device");
}

/// <summary>
/// ��ȯ �罽 ����ü�� ����
/// </summary>
void GraphicsEngine::CreateChainValue()
{
	RECT windowSize;
	assert(GetWindowRect(hwnd, &windowSize) && "cannot get window rectangle");

	// �ϵ��� 4X MSAA ǰ�� ������ �����ϴ���, �������� �ʴٸ� ����
	HRESULT hr;
	hr = d3d11Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &this->m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	// ����  ü�� ����ü
	DXGI_SWAP_CHAIN_DESC chainDesc;

	// ������ ũ�⸦ �������� ������ ��ŭ ����
	chainDesc.BufferDesc.Width = windowSize.right - windowSize.left;
	chainDesc.BufferDesc.Height = windowSize.bottom - windowSize.top;
	// ȭ�� ���� ���� (������) (����, �и�)
	chainDesc.BufferDesc.RefreshRate.Numerator = 60;
	chainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// ���÷��� ����
	chainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// ���� ǥ��ȭ ������ ���� ǥ�� ������ ����
	chainDesc.SampleDesc.Count = 4;
	chainDesc.SampleDesc.Quality = this->m4xMsaaQuality - 1;
	// ������ �뵵 (�ĸ� ����)
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// ������ ����
	chainDesc.BufferCount = 1;
	// ����� ǥ���� ������ �ڵ�
	chainDesc.OutputWindow = this->hwnd;
	// â��� ����
	chainDesc.Windowed = true;
	// ��ȯ ȿ�� ���� (����̽��� �˾Ƽ� ȿ������ ��� ����)
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// �߰� �÷���
	chainDesc.Flags = 0;

	// ���� ü�� ������ ���� COM ����
	// ����̽� �������̽� ��������
	IDXGIDevice* dxgiDevice = nullptr;
	hr = (d3d11Device->QueryInterface(
		__uuidof(IDXGIDevice),
		reinterpret_cast<void**>(&dxgiDevice)));
	assert(hr == S_OK && "cannot get DXGI device");

	// ����̽� ��� ��������
	IDXGIAdapter* dxgiAdapter = nullptr;
	hr = (dxgiDevice->GetParent(
		__uuidof(IDXGIAdapter),
		reinterpret_cast<void**>(&dxgiAdapter)));
	assert(hr == S_OK && "cannot get DXGI adapter");

	// ����̽� ���丮 ��������
	IDXGIFactory* dxgiFactory = nullptr;
	hr = (dxgiAdapter->GetParent(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&dxgiFactory)));
	assert(hr == S_OK && "cannot get DXGI factory");

	// ���� ü�� ����
	hr = (dxgiFactory->CreateSwapChain(this->d3d11Device, &chainDesc, &this->swapChain));
	assert(hr == S_OK && "cannot create swapChain");

	// ����� �������̽� ����
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
}

/// <summary>
/// ���� �� ����
/// </summary>
void GraphicsEngine::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr;

	// ���� ü�ο��� ���۸� ������
		// 1. ���� ( ���۰� �������� ��� �߿� )
		// 2. ������ �������̽� ���� ( ���� 2���� �ؽ��� ���� ������ �ش�. )
		// 3. �޾ƿ� �� ����
	hr = this->swapChain->GetBuffer(
		0,
		__uuidof(backBuffer),
		reinterpret_cast<void**>(&backBuffer)
	);
	assert(hr == S_OK && "cannot get buffer");

	// ���� Ÿ�� �� ����
		// 1. �� ����
		// 2. ���� ���� ( �� ������ �˾Ƽ� ���� )
		// 3. ���� Ÿ��
	hr = this->d3d11Device->CreateRenderTargetView(
		backBuffer,
		nullptr,
		&this->renderTargetView
	);
	assert(hr == S_OK && "cannot create RenderTargetView");

	// ����� �� ���� �������̽� ��ȯ	
	hr = backBuffer->Release();
	assert(hr == S_OK && "cannot release backBuffer");

}

/// <summary>
/// ���� ���ٽ� ���ۿ� �� ���� ����
/// </summary>
void GraphicsEngine::CreateDepthStencilBufferAndView()
{
	HRESULT hr = S_OK;

	RECT windowSize;
	assert(GetWindowRect(hwnd, &windowSize) && "cannot get window rectangle");

	// ����ü �� ä���
	D3D11_TEXTURE2D_DESC depthStancilDesc;
	// ������ ������
	depthStancilDesc.Width = windowSize.right - windowSize.left;
	depthStancilDesc.Height = windowSize.bottom - windowSize.top;
	// �Ӹ� ���� ���� ���� ( ���� ���ٽ� ���ۿ����� �ϳ��� �ʿ� )
	depthStancilDesc.MipLevels = 1;
	// �ؽ��� �迭�� ���� ( ���� ���ٽ� ���ۿ����� �ϳ��� �ʿ� )
	depthStancilDesc.ArraySize = 1;
	// �ؼ� ����
	depthStancilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ���� ǥ�� ������ ����. �ݵ�� ���� ���� ��ġ�ؾ� �Ѵ�.
	// ���� m4xmass ����� �ݵ�� �����ϹǷ�
	depthStancilDesc.SampleDesc.Count = 4;
	depthStancilDesc.SampleDesc.Quality = this->m4xMsaaQuality - 1;
	// �ؽ����� �뵵 ( �ڼ��� ������ MSDN / ��å 167������  ���� )
	depthStancilDesc.Usage = D3D11_USAGE_DEFAULT;
	// ������ ���� ��� ���ε� �� ��
	depthStancilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// CPU�� �ش� ���ۿ� �����ϴ� ���. ���� ���ٽ� ���۴� CPU ������ �ʿ� �����Ƿ� 0
	depthStancilDesc.CPUAccessFlags = 0;
	// ��Ÿ �÷���. ���� ���ٽ� ���ۿ����� �۵����� �ʴ´�.
	depthStancilDesc.MiscFlags = 0;

	hr = this->d3d11Device->CreateTexture2D(
		&depthStancilDesc,
		nullptr,
		&this->depthStancilBuffer
	);
	assert(hr == S_OK && "cannot create depth-stancil buffer");

	hr = d3d11Device->CreateDepthStencilView(
		this->depthStancilBuffer,
		0,
		&this->depthStancilView
	);
	assert(hr == S_OK && "cannot create depth-stancil view");
}

/// <summary>
/// ������ �並 ������ ���������ο� ���´�.
/// </summary>
void GraphicsEngine::BindView()
{
	// �並 ������ ���������ε� ���ε�
		// 1. ���ε� �� ������ ����� ���� ( �⺻������ �ϳ����� ����� ������ �� ���� �ִ�. )
		// 2. ���������ε� ���ε� �� ���� �� �迭�� ù �ּ� ( �ϳ��ۿ� �����Ƿ� �� �ּҸ� �ָ� �ȴ�. )
		// 3. ���� ���ٽ� ��
	this->d3d11DeviceContext->OMSetRenderTargets(
		1,
		&this->renderTargetView,
		this->depthStancilView
	);
}

/// <summary>
/// ���� Ÿ�� �� �ʱ�ȭ
/// </summary>
void GraphicsEngine::ClearRenderTargetView()
{
	// �ӽ� �� ( R G B A )
	float bgRed[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	// ���� Ÿ���� ������ ������ �ʱ�ȭ
	this->d3d11DeviceContext->ClearRenderTargetView(
		this->renderTargetView,
		bgRed
	);
}

/// <summary>
/// ���� ���ٽ� �� �ʱ�ȭ
/// </summary>
void GraphicsEngine::ClearDepthStencilView()
{
	// ���� ���ٽ� �� �ʱ�ȭ
	this->d3d11DeviceContext->ClearDepthStencilView(
		this->depthStancilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
}
