#include "GraphicsEngine.h"
#include <minwinbase.h>
#include "DXTKFont.h"

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
	, matrixBuffer(nullptr)
	, useMSAA(true)
	, writerDSS(nullptr)
	, writerRS(nullptr)
	, writer(nullptr)
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
	CreateViewport();
	BindView();
	CreateWriter();
}

/// <summary>
/// ȭ���� Ư�� ������ �ʱ�ȭ
/// </summary>
void GraphicsEngine::RenderClearView()
{
	ClearRenderTargetView();
	ClearDepthStencilView();
}

/// <summary>
/// �ӽ� ������Ʈ ����
/// </summary>
void GraphicsEngine::RenderTestThing(PipeLine& _pipline)
{

 	this->d3d11DeviceContext->DrawIndexed(36, 0, 0);
}

void GraphicsEngine::RenderByIndex(PipeLine& _pipline, int _indexSize)
{
 	this->d3d11DeviceContext->DrawIndexed(_indexSize, 0, 0);
}

/// <summary>
/// D3D11 ����̽��� ����̽� ���ؽ�Ʈ ����
/// </summary>
void GraphicsEngine::CreateD3D11DeviceContext()
{
	HRESULT hr;
	// D3D11 ����̽� ����
	hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		this->createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
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
	DXGI_SWAP_CHAIN_DESC chainDesc = {};

	chainDesc.BufferDesc.Width = windowSize.right - windowSize.left;
	chainDesc.BufferDesc.Height = windowSize.bottom - windowSize.top;
	chainDesc.BufferDesc.RefreshRate.Numerator = 60;
	chainDesc.BufferDesc.RefreshRate.Denominator = 1;
	chainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (this->useMSAA) 
	{
		chainDesc.SampleDesc.Count = 4;
		chainDesc.SampleDesc.Quality = this->m4xMsaaQuality - 1;
	}
	else
	{
		chainDesc.SampleDesc.Count = 1;
		chainDesc.SampleDesc.Quality = 0;
	}


	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = 1;
	chainDesc.OutputWindow = this->hwnd;
	chainDesc.Windowed = true;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	chainDesc.Flags = 0;

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
	hr = this->swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	assert(hr == S_OK && "cannot get buffer");

	// ���� Ÿ�� �� ����
	hr = this->d3d11Device->CreateRenderTargetView(
		backBuffer,
		0,
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
	D3D11_TEXTURE2D_DESC depthStancilDesc = {};
	depthStancilDesc.Width = windowSize.right - windowSize.left;
	depthStancilDesc.Height = windowSize.bottom - windowSize.top;
	depthStancilDesc.MipLevels = 1;
	depthStancilDesc.ArraySize = 1;
	depthStancilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (this->useMSAA) 
	{
		depthStancilDesc.SampleDesc.Count = 4;
		depthStancilDesc.SampleDesc.Quality = this->m4xMsaaQuality - 1;
	}
	else
	{
		depthStancilDesc.SampleDesc.Count = 1;
		depthStancilDesc.SampleDesc.Quality = 0;
	}


	depthStancilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStancilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStancilDesc.CPUAccessFlags = 0;
	depthStancilDesc.MiscFlags = 0;

	// ���� ���ٽ� ���� ����
	hr = this->d3d11Device->CreateTexture2D(
		&depthStancilDesc,
		nullptr,
		&this->depthStancilBuffer
	);
	assert(hr == S_OK && "cannot create depth-stancil buffer");

	// ���� ���ٽ� �� ����
	hr = d3d11Device->CreateDepthStencilView(
		this->depthStancilBuffer,
		0,
		&this->depthStancilView
	);
	assert(hr == S_OK && "cannot create depth-stancil view");
}

/// <summary>
/// ����Ʈ ����
/// </summary>
void GraphicsEngine::CreateViewport()
{
	RECT windowSize;
	assert(GetWindowRect(hwnd, &windowSize) && "cannot get window rectangle");
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(windowSize.right - windowSize.left);
	vp.Height = static_cast<FLOAT>(windowSize.bottom - windowSize.top);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->d3d11DeviceContext->RSSetViewports(1, &vp);
}

/// <summary>
/// ������ �並 ������ ���������ο� ���´�.
/// </summary>
void GraphicsEngine::BindView()
{
	// �並 ������ ���������ο� ���ε�
	this->d3d11DeviceContext->OMSetRenderTargets(
		1,
		&this->renderTargetView,
		this->depthStancilView
	);
}

void GraphicsEngine::CreateWriter()
{
	// ��Ʈ�� DSS
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// ���̹��ۿ� ����� �Ѵ�
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	this->d3d11Device->CreateDepthStencilState(&equalsDesc, &writerDSS);

	// Render State �� Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	this->d3d11Device->CreateRasterizerState(&solidDesc, &writerRS);
	this->writer = new DXTKFont();
	this->writer->Create(this->d3d11Device, this->writerRS, this->writerDSS);
}

/// <summary>
/// Input Layer�� �����Ѵ�
/// </summary>
void GraphicsEngine::CreateInputLayer(ID3D11InputLayout** _inputLayout, ID3D11VertexShader** _vertexShader, ID3D11PixelShader** _pixelShader)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC defaultInputLayerDECS[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* vsByteCode;
	ID3DBlob* psByteCode;
	ID3DBlob* compileError;

	// TODO : ����η� �ٲٱ�
	hr = D3DCompileFromFile(
		L"C:\\Users\\User\\project\\Engine\\EngineHH\\src\\D3DGraphics\\VertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS",
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&vsByteCode,
		&compileError
	);
	assert(SUCCEEDED(hr) && "cannot Compile Vertex Shader");
	
	// TODO : ����η� �ٲٱ�
	hr = D3DCompileFromFile(
		L"C:\\Users\\User\\project\\Engine\\EngineHH\\src\\D3DGraphics\\PixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS",
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&psByteCode,
		&compileError
	);
	assert(SUCCEEDED(hr) && "cannot Compile Pixel Shader");

	this->d3d11Device->CreateVertexShader(vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), nullptr, _vertexShader);
	this->d3d11Device->CreatePixelShader(psByteCode->GetBufferPointer(), psByteCode->GetBufferSize(), nullptr, _pixelShader);

	hr = this->d3d11Device->CreateInputLayout(
		defaultInputLayerDECS,
		2,
		vsByteCode->GetBufferPointer(),
		vsByteCode->GetBufferSize(),
		_inputLayout
	);
	assert(SUCCEEDED(hr) && "cannot create inpur layer");

	D3D11_BUFFER_DESC mbd = {};
	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(MatrixBufferType);
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	this->d3d11Device->CreateBuffer(&mbd, nullptr, &matrixBuffer);
}

/// <summary>
/// ���� Ÿ�� �� �ʱ�ȭ
/// </summary>
void GraphicsEngine::ClearRenderTargetView()
{
	// �ӽ� �� ( R G B A )
	float bgRed[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

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

/// <summary>
/// �����Ͷ����� ����
/// </summary>
/// <param name="_rasterizerState">��ȯ ���� �����Ͷ�����</param>
void GraphicsEngine::CreateRasterizerState(ID3D11RasterizerState** _rasterizerState)
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = true;

	hr = this->d3d11Device->CreateRasterizerState(&rsDesc, _rasterizerState);
	assert(SUCCEEDED(hr) && "cannot create Rasterizser State");
}

/// <summary>
/// �Ķ���� ����
/// </summary>
/// <param name="_w">���� TM</param>
/// <param name="_v">����Ʈ TM</param>
/// <param name="_p">�������� TM</param>
void GraphicsEngine::SetParameter(DirectX::XMMATRIX _w, DirectX::XMMATRIX _v, DirectX::XMMATRIX _p)
{
	HRESULT hr;

	_w = DirectX::XMMatrixTranspose(_w);
	_v = DirectX::XMMatrixTranspose(_v);
	_p = DirectX::XMMatrixTranspose(_p);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = this->d3d11DeviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	MatrixBufferType* dataptr = (MatrixBufferType*)mappedResource.pData;
	
	dataptr->world = _w;
	dataptr->view = _v;
	dataptr->proj = _p;

	this->d3d11DeviceContext->Unmap(matrixBuffer, 0);

	this->d3d11DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
}

/// <summary>
/// ���������� ���ε�
/// </summary>
/// <param name="_pipline"></param>
void GraphicsEngine::BindPipeline(PipeLine& _pipline)
{
	this->d3d11DeviceContext->IASetInputLayout(_pipline.inputLayout);
	this->d3d11DeviceContext->IASetPrimitiveTopology(_pipline.primitiveTopology);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	this->d3d11DeviceContext->IASetIndexBuffer(_pipline.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	this->d3d11DeviceContext->IASetVertexBuffers(0, 1, &_pipline.vertexBuffer, &stride, &offset);
	this->d3d11DeviceContext->RSSetState(_pipline.rasterizerState);
	this->d3d11DeviceContext->VSSetShader(_pipline.vertexShader, nullptr, 0);
	this->d3d11DeviceContext->PSSetShader(_pipline.pixelShader, nullptr, 0);
}

/// <summary>
/// ���� ���ۿ� ���� �߰�
/// </summary>
void GraphicsEngine::CreateVertexBuffer(Vertex* _verteies, size_t _size, ID3D11Buffer** _vertexbuffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vb = {};

	vb.Usage = D3D11_USAGE_IMMUTABLE;
	vb.ByteWidth = static_cast<UINT>(_size);
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = _verteies;

	hr = this->d3d11Device->CreateBuffer(
		&vb,
		&initData,
		_vertexbuffer
	);
	assert(SUCCEEDED(hr) && "cannot create vertex buffer");
}

/// <summary>
/// �ε��� ���� ����
/// </summary>
/// <param name="_indices">�ε��� �迭</param>
/// <param name="_size">�迭�� ������</param>
/// <param name="_indexbuffer">���۸� ��ȯ���� ������</param>
void GraphicsEngine::CreateIndexBuffer(UINT* _indices, size_t _size, ID3D11Buffer** _indexbuffer)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = static_cast<UINT>(_size);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = _indices;

	hr = this->d3d11Device->CreateBuffer(&ibd, &iinitData, _indexbuffer);
	assert(SUCCEEDED(hr) && "cannot create Index Buffer");
}

/// <summary>
/// �׸��� �۾��� �����ϰ� ���
/// </summary>
void GraphicsEngine::endDraw()
{
	this->swapChain->Present(0, 0);
}

/// <summary>
/// �׸��� �۾� ���۽� �ʱ�ȭ
/// </summary>
void GraphicsEngine::begineDraw()
{
	RenderClearView();
}
