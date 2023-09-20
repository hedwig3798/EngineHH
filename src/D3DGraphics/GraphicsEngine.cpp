#include "GraphicsEngine.h"
#include <minwinbase.h>

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
{
}

GraphicsEngine::~GraphicsEngine()
{
}

/// <summary>
/// 그래픽 엔진 초기화
/// </summary>
/// <param name="_hwnd">윈도우 핸들</param>
void GraphicsEngine::Initialize(HWND _hwnd)
{
	this->hwnd = _hwnd;

	CreateD3D11DeviceContext();
	CreateChainValue();
	CreateRenderTargetView();
	CreateDepthStencilBufferAndView();
}

/// <summary>
/// 화면을 특정 색으로 초기화
/// </summary>
void GraphicsEngine::RenderClearView()
{
	ClearRenderTargetView();
	ClearDepthStencilView();
}

/// <summary>
/// 임시 오브젝트 렌더
/// </summary>
void GraphicsEngine::RenderTestThing(PipeLine& _pipline)
{
	this->d3d11DeviceContext->IASetInputLayout(_pipline.inputLayout);
	this->d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	this->d3d11DeviceContext->IAGetVertexBuffers(0, 1, &_pipline.vertexBuffer, &stride, &offset);
	this->d3d11DeviceContext->IASetIndexBuffer(_pipline.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	this->d3d11DeviceContext->VSSetShader(_pipline.vertexShader, nullptr, 0);
	this->d3d11DeviceContext->PSSetShader(_pipline.pixelShader, nullptr, 0);
	this->d3d11DeviceContext->DrawIndexed(36, 0, 0);
}

/// <summary>
/// D3D11 디바이스와 디바이스 컨텍스트 생성
/// </summary>
void GraphicsEngine::CreateD3D11DeviceContext()
{
	// D3D11 디바이스 생성
	HRESULT hr;

	hr = D3D11CreateDevice(
		0, // 기본 어뎁터
		D3D_DRIVER_TYPE_HARDWARE,
		0, // 소프트웨어 장치 사용 하지 않음
		this->createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION, // D3D11 버전
		&this->d3d11Device,
		&this->featureLevel,
		&this->d3d11DeviceContext
	);
	assert(hr == S_OK && "cannot create d3d11 device");
}

/// <summary>
/// 교환 사슬 구조체의 생성
/// </summary>
void GraphicsEngine::CreateChainValue()
{
	RECT windowSize;
	assert(GetWindowRect(hwnd, &windowSize) && "cannot get window rectangle");

	// 하드웨어가 4X MSAA 품질 수준을 지원하는지, 지원하지 않다면 종료
	HRESULT hr;
	hr = d3d11Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &this->m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	// 스왑  체인 구조체
	DXGI_SWAP_CHAIN_DESC chainDesc;

	// 버퍼의 크기를 윈도우의 사이즈 만큼 설정
	chainDesc.BufferDesc.Width = windowSize.right - windowSize.left;
	chainDesc.BufferDesc.Height = windowSize.bottom - windowSize.top;
	// 화면 갱신 비율 (프레임) (분자, 분모)
	chainDesc.BufferDesc.RefreshRate.Numerator = 60;
	chainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// 디스플레이 포멧
	chainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 다중 표본화 추출을 위한 표본 갯수와 수준
	chainDesc.SampleDesc.Count = 4;
	chainDesc.SampleDesc.Quality = this->m4xMsaaQuality - 1;
	// 버퍼의 용도 (후면 버퍼)
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 버퍼의 갯수
	chainDesc.BufferCount = 1;
	// 결과를 표시할 윈도우 핸들
	chainDesc.OutputWindow = this->hwnd;
	// 창모드 여부
	chainDesc.Windowed = true;
	// 교환 효과 지정 (디바이스가 알아서 효율적인 방법 설정)
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// 추가 플래그
	chainDesc.Flags = 0;

	// 스왑 체인 생성을 위한 COM 질의
	// 디바이스 인터페이스 가져오기
	IDXGIDevice* dxgiDevice = nullptr;
	hr = (d3d11Device->QueryInterface(
		__uuidof(IDXGIDevice),
		reinterpret_cast<void**>(&dxgiDevice)));
	assert(hr == S_OK && "cannot get DXGI device");

	// 디바이스 어뎁터 가져오기
	IDXGIAdapter* dxgiAdapter = nullptr;
	hr = (dxgiDevice->GetParent(
		__uuidof(IDXGIAdapter),
		reinterpret_cast<void**>(&dxgiAdapter)));
	assert(hr == S_OK && "cannot get DXGI adapter");

	// 디바이스 펙토리 가져오기
	IDXGIFactory* dxgiFactory = nullptr;
	hr = (dxgiAdapter->GetParent(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&dxgiFactory)));
	assert(hr == S_OK && "cannot get DXGI factory");

	// 스왑 체인 생성
	hr = (dxgiFactory->CreateSwapChain(this->d3d11Device, &chainDesc, &this->swapChain));
	assert(hr == S_OK && "cannot create swapChain");

	// 사용한 인터페이스 제거
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
}

/// <summary>
/// 렌더 뷰 생성
/// </summary>
void GraphicsEngine::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr;

	// 스왑 체인에서 버퍼를 가져옴
		// 1. 색인 ( 버퍼가 여려개인 경우 중요 )
		// 2. 버퍼의 인터페이스 형식 ( 보통 2차원 텍스쳐 버퍼 형식을 준다. )
		// 3. 받아올 백 버퍼
	hr = this->swapChain->GetBuffer(
		0,
		__uuidof(backBuffer),
		reinterpret_cast<void**>(&backBuffer)
	);
	assert(hr == S_OK && "cannot get buffer");

	// 렌더 타겟 뷰 생성
		// 1. 백 버퍼
		// 2. 버퍼 형식 ( 널 지정시 알아서 지정 )
		// 3. 렌더 타겟
	hr = this->d3d11Device->CreateRenderTargetView(
		backBuffer,
		nullptr,
		&this->renderTargetView
	);
	assert(hr == S_OK && "cannot create RenderTargetView");

	// 사용한 백 버퍼 인터페이스 반환	
	hr = backBuffer->Release();
	assert(hr == S_OK && "cannot release backBuffer");

}

/// <summary>
/// 깊이 스텐실 버퍼와 뷰 생성 생성
/// </summary>
void GraphicsEngine::CreateDepthStencilBufferAndView()
{
	HRESULT hr = S_OK;

	RECT windowSize;
	assert(GetWindowRect(hwnd, &windowSize) && "cannot get window rectangle");

	// 구조체 값 채우기
	D3D11_TEXTURE2D_DESC depthStancilDesc;
	// 윈도우 사이즈
	depthStancilDesc.Width = windowSize.right - windowSize.left;
	depthStancilDesc.Height = windowSize.bottom - windowSize.top;
	// 밉맵 개수 추후 공부 ( 깊이 스텐실 버퍼에서는 하나만 필요 )
	depthStancilDesc.MipLevels = 1;
	// 텍스쳐 배열의 갯수 ( 깊이 스텐실 버퍼에서는 하나만 필요 )
	depthStancilDesc.ArraySize = 1;
	// 텍셀 형싱
	depthStancilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 다중 표본 갯수와 수준. 반드시 렌더 대상과 일치해야 한다.
	// 현재 m4xmass 기능을 반드시 지원하므로
	depthStancilDesc.SampleDesc.Count = 4;
	depthStancilDesc.SampleDesc.Quality = this->m4xMsaaQuality - 1;
	// 텍스쳐의 용도 ( 자세한 내용은 MSDN / 용책 167페이지  참고 )
	depthStancilDesc.Usage = D3D11_USAGE_DEFAULT;
	// 파이프 라인 어디에 바인드 할 지
	depthStancilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// CPU가 해당 버퍼에 접근하는 방법. 깊이 스텐실 버퍼는 CPU 접근이 필요 없으므로 0
	depthStancilDesc.CPUAccessFlags = 0;
	// 기타 플래그. 깊이 스텐실 버퍼에서는 작동하지 않는다.
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
/// 생성된 뷰를 렌더링 파이프라인에 묶는다.
/// </summary>
void GraphicsEngine::BindView()
{
	// 뷰를 렌더링 파이프라인데 바인드
		// 1. 바인드 할 렌더링 대상의 갯수 ( 기본적으로 하나지만 대상이 여러개 일 수도 있다. )
		// 2. 파이프라인데 바인드 할 렌더 뷰 배열의 첫 주소 ( 하나밖에 없으므로 그 주소를 주면 된다. )
		// 3. 깊이 스텐실 뷰
	this->d3d11DeviceContext->OMSetRenderTargets(
		1,
		&this->renderTargetView,
		this->depthStancilView
	);
}

/// <summary>
/// Input Layer를 생성한다
/// </summary>
void GraphicsEngine::CreateInputLayer(ID3D11InputLayout* _inputLayout, ID3D11VertexShader* _vertexShader, ID3D11PixelShader* _pixelShader)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC defaultInputLayerDECS[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* vsByteCode;
	ID3DBlob* psByteCode;
	ID3DBlob* compileError;

	// TODO : 상대경로로 바꾸기
	hr = D3DCompileFromFile(
		L"C:\\Users\\User\\project\\Engine\\EngineHH\\src\\D3DGraphics\\VertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsByteCode,
		&compileError
	);
	assert(hr == S_OK && "cannot Compile Vertex Shader");
	
	// TODO : 상대경로로 바꾸기
	hr = D3DCompileFromFile(
		L"C:\\Users\\User\\project\\Engine\\EngineHH\\src\\D3DGraphics\\PixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psByteCode,
		&compileError
	);
	assert(hr == S_OK && "cannot Compile Pixel Shader");

	this->d3d11Device->CreateVertexShader(vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), nullptr, &_vertexShader);
	this->d3d11Device->CreatePixelShader(psByteCode->GetBufferPointer(), psByteCode->GetBufferSize(), nullptr, &_pixelShader);

	hr = this->d3d11Device->CreateInputLayout(
		defaultInputLayerDECS,
		2,
		vsByteCode->GetBufferPointer(),
		vsByteCode->GetBufferSize(),
		&_inputLayout
	);
	assert(hr == S_OK && "cannot create inpur layer");

	D3D11_BUFFER_DESC mbd;
	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(MatrixBufferType);
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	this->d3d11Device->CreateBuffer(&mbd, nullptr, &matrixBuffer);
}

/// <summary>
/// 렌더 타겟 뷰 초기화
/// </summary>
void GraphicsEngine::ClearRenderTargetView()
{
	// 임시 색 ( R G B A )
	float bgRed[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 렌더 타겟을 지정한 색으로 초기화
	this->d3d11DeviceContext->ClearRenderTargetView(
		this->renderTargetView,
		bgRed
	);
}

/// <summary>
/// 깊이 스텐실 뷰 초기화
/// </summary>
void GraphicsEngine::ClearDepthStencilView()
{
	// 깊이 스텐실 뷰 초기화
	this->d3d11DeviceContext->ClearDepthStencilView(
		this->depthStancilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
}

void GraphicsEngine::CreateRasterizerState(ID3D11RasterizerState* _rasterizerState)
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = true;

	hr = this->d3d11Device->CreateRasterizerState(&rsDesc, &_rasterizerState);
	assert(SUCCEEDED(hr) && "cannot create Rasterizser State");

	this->d3d11DeviceContext->RSSetState(_rasterizerState);
}

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
/// 정점 버퍼에 정점 추가
/// </summary>
void GraphicsEngine::CreateVertexBuffer(Vertex* _verteies, size_t _size, ID3D11Buffer* _vertexbuffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vb;

	vb.Usage = D3D11_USAGE_IMMUTABLE;
	vb.ByteWidth = static_cast<UINT>(_size);
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = _verteies;

	hr = this->d3d11Device->CreateBuffer(
		&vb,
		&initData,
		&_vertexbuffer
	);
	assert(hr == S_OK && "cannot create vertex buffer");
}

void GraphicsEngine::CreateIndexBuffer(UINT* _indices, size_t _size, ID3D11Buffer* _indexbuffer)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = static_cast<UINT>(_size);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = _indices;

	hr = this->d3d11Device->CreateBuffer(&ibd, &iinitData, &_indexbuffer);
	assert(SUCCEEDED(hr) && "cannot create Index Buffer");
}

void GraphicsEngine::endDraw()
{
	this->swapChain->Present(0, 0);
}

void GraphicsEngine::begineDraw()
{
	RenderClearView();
}
