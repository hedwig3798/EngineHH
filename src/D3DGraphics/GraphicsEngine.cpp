#include "GraphicsEngine.h"
#include <minwinbase.h>
#include "DXTKFont.h"
#include "DirectXTex.h"
#include "DDSTextureLoader.h"
#include "LightHelper.h"
#include "RenderObject.h"
#include "FbxLoader.h"
#include "FObject.h"

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
	, lightBuffer(nullptr)
	, fbxLoader(nullptr)
	, boneBuffer(nullptr)
{
}

GraphicsEngine::~GraphicsEngine()
{
	this->writerDSS->Release();
	this->writerRS->Release();
	delete this->writer;

	this->matrixBuffer->Release();
	this->depthStancilView->Release();
	this->depthStancilBuffer->Release();
	this->renderTargetView->Release();
	this->swapChain->Release();
	this->d3d11Device->Release();
	this->d3d11DeviceContext->Release();
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
	CreateViewport();
	BindView();
	CreateWriter();
	CreateMatrixBuffer();
	CreateLightingBffer();
	CreateBoneBuffer();

	this->fbxLoader = new FbxLoader();
	this->fbxLoader->Initalize();
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

	this->d3d11DeviceContext->DrawIndexed(36, 0, 0);
}

void GraphicsEngine::RenderByIndex(PipeLine& _pipline, int _indexSize)
{
	this->d3d11DeviceContext->DrawIndexed(_indexSize, 0, 0);
}

/// <summary>
/// D3D11 디바이스와 디바이스 컨텍스트 생성
/// </summary>
void GraphicsEngine::CreateD3D11DeviceContext()
{
	HRESULT hr;
	// D3D11 디바이스 생성
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
	hr = this->swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	assert(hr == S_OK && "cannot get buffer");

	// 렌더 타겟 뷰 생성
	hr = this->d3d11Device->CreateRenderTargetView(
		backBuffer,
		0,
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

	// 깊이 스텐실 버퍼 생성
	hr = this->d3d11Device->CreateTexture2D(
		&depthStancilDesc,
		nullptr,
		&this->depthStancilBuffer
	);
	assert(hr == S_OK && "cannot create depth-stancil buffer");

	// 깊이 스텐실 뷰 생성
	hr = d3d11Device->CreateDepthStencilView(
		this->depthStancilBuffer,
		0,
		&this->depthStancilView
	);
	assert(hr == S_OK && "cannot create depth-stancil view");
}

/// <summary>
/// 뷰포트 생성
/// </summary>
void GraphicsEngine::CreateViewport()
{
	RECT windowSize;
	assert(GetWindowRect(hwnd, &windowSize) && "cannot get window rectangle");

	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<FLOAT>(windowSize.right - windowSize.left);
	vp.Height = static_cast<FLOAT>(windowSize.bottom - windowSize.top);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->d3d11DeviceContext->RSSetViewports(1, &vp);
}

/// <summary>
/// 생성된 뷰를 렌더링 파이프라인에 묶는다.
/// </summary>
void GraphicsEngine::BindView()
{
	// 뷰를 렌더링 파이프라인에 바인드
	this->d3d11DeviceContext->OMSetRenderTargets(
		1,
		&this->renderTargetView,
		this->depthStancilView
	);
}

void GraphicsEngine::CreateWriter()
{
	// 폰트용 DSS
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	this->d3d11Device->CreateDepthStencilState(&equalsDesc, &writerDSS);

	// Render State 중 Rasterizer State
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

std::vector<byte> GraphicsEngine::Read(std::string File)
{
	std::vector<byte> Text;
	std::fstream file(File, std::ios::in | std::ios::ate | std::ios::binary);

	if (file.is_open()) {
		Text.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(&Text[0]), Text.size());
		file.close();
	}

	return Text;
}

/// <summary>
/// Input Layer를 생성한다
/// </summary>
void GraphicsEngine::CreateInputLayer(PipeLine& _pipline, D3D11_INPUT_ELEMENT_DESC* _defaultInputLayerDECS, std::wstring _path[], UINT _numberOfElement)
{
	HRESULT hr = S_OK;
	std::string vs = "";
	vs.assign(_path[0].begin(), _path[0].end());
	auto vsByteCode = Read(vs);

	std::string ps = "";
	ps.assign(_path[1].begin(), _path[1].end());
	auto psByteCode = Read(ps);


	hr = this->d3d11Device->CreateVertexShader(vsByteCode.data(), vsByteCode.size(), nullptr, &_pipline.vertexShader);
	hr = this->d3d11Device->CreatePixelShader(psByteCode.data(), psByteCode.size(), nullptr, &_pipline.pixelShader);

	hr = this->d3d11Device->CreateInputLayout(
		_defaultInputLayerDECS,
		_numberOfElement,
		vsByteCode.data(),
		vsByteCode.size(),
		&_pipline.inputLayout
	);
	assert(SUCCEEDED(hr) && "cannot create input layer");
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

/// <summary>
/// 레스터라이저 생성
/// </summary>
/// <param name="_rasterizerState">반환 받을 레스터라이저</param>
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

void GraphicsEngine::CreateMatrixBuffer()
{
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
/// 파라미터 설정
/// </summary>
/// <param name="_w">월드 TM</param>
/// <param name="_v">뷰포트 TM</param>
/// <param name="_p">프로젝션 TM</param>
void GraphicsEngine::BindMatrixParameter(DirectX::XMMATRIX _w, DirectX::XMMATRIX _v, DirectX::XMMATRIX _p, Material _material)
{
	HRESULT hr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = this->d3d11DeviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	MatrixBufferType* dataptr = (MatrixBufferType*)mappedResource.pData;

	dataptr->world = _w;
	dataptr->wvp = _w * _v * _p;
	dataptr->worldInversTranspose = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, _w));

	dataptr->world = DirectX::XMMatrixTranspose(_w);
	dataptr->wvp = DirectX::XMMatrixTranspose(dataptr->wvp);
	dataptr->worldInversTranspose = DirectX::XMMatrixTranspose(dataptr->worldInversTranspose);

	dataptr->material = _material;

	this->d3d11DeviceContext->Unmap(matrixBuffer, 0);
	this->d3d11DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
}


void GraphicsEngine::CreateBoneBuffer()
{
	D3D11_BUFFER_DESC mbd = {};
	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(BonesBufferType);
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	this->d3d11Device->CreateBuffer(&mbd, nullptr, &this->boneBuffer);
}

void GraphicsEngine::BindBonesData(std::vector<RenderObject*>& _bones, DirectX::XMMATRIX _worldTM)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = this->d3d11DeviceContext->Map(this->boneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	BonesBufferType* dataptr = static_cast<BonesBufferType*>(mappedResource.pData);
	for (int i = 0; i < (int)_bones.size(); i++)
	{
		DirectX::XMMATRIX boneWorldTM = _bones[i]->nodeTM;
		DirectX::XMMATRIX boneNodeTM = _bones[i]->originalNodeTM;

		DirectX::XMMATRIX skinWorldTM = _worldTM;
		DirectX::XMMATRIX skinWorldTMInverse = DirectX::XMMatrixInverse(nullptr, skinWorldTM);

		DirectX::XMMATRIX boneoffsetTM = boneNodeTM * skinWorldTMInverse;
		DirectX::XMMATRIX boneoffsetTM_Inverse = DirectX::XMMatrixInverse(nullptr, boneoffsetTM);

		DirectX::XMMATRIX finalboneTM = boneoffsetTM_Inverse * boneWorldTM;

		dataptr->bones[i] = DirectX::XMMatrixTranspose(finalboneTM);
	}
	this->d3d11DeviceContext->Unmap(this->boneBuffer, 0);
	this->d3d11DeviceContext->VSSetConstantBuffers(2, 1, &boneBuffer);
}

void GraphicsEngine::CreateLightingBffer()
{
	D3D11_BUFFER_DESC mbd = {};
	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(LightingBufferType);
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	this->d3d11Device->CreateBuffer(&mbd, nullptr, &this->lightBuffer);
}

void GraphicsEngine::BindLightingParameter(DirectionalLight _directionLight[], UINT _lightCount, DirectX::XMFLOAT3 _cameraPos)
{
	HRESULT hr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = this->d3d11DeviceContext->Map(this->lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	LightingBufferType* dataptr = static_cast<LightingBufferType*>(mappedResource.pData);

	dataptr->dirLights[0] = _directionLight[0];
	dataptr->dirLights[1] = _directionLight[1];
	dataptr->dirLights[2] = _directionLight[2];

	dataptr->lightCount = _lightCount;

	dataptr->eyePosW = _cameraPos;

	this->d3d11DeviceContext->Unmap(this->lightBuffer, 0);
	this->d3d11DeviceContext->PSSetConstantBuffers(1, 1, &this->lightBuffer);
}

/// <summary>
/// 파이프라인 바인딩
/// </summary>
/// <param name="_pipline"></param>
void GraphicsEngine::BindPipeline(PipeLine& _pipline)
{
	this->d3d11DeviceContext->IASetInputLayout(_pipline.inputLayout);
	this->d3d11DeviceContext->IASetPrimitiveTopology(_pipline.primitiveTopology);

	UINT stride = _pipline.vertexStructSize;
	UINT offset = 0;

	this->d3d11DeviceContext->IASetIndexBuffer(_pipline.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	this->d3d11DeviceContext->IASetVertexBuffers(0, 1, &_pipline.vertexBuffer, &stride, &offset);
	this->d3d11DeviceContext->RSSetState(_pipline.rasterizerState);
	this->d3d11DeviceContext->VSSetShader(_pipline.vertexShader, nullptr, 0);
	this->d3d11DeviceContext->PSSetShader(_pipline.pixelShader, nullptr, 0);
}

void GraphicsEngine::WriteText(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text)
{
	this->writer->DrawTextColor(x, y, color, text);
}

/// <summary>
/// 텍스쳐 데이터 생성
/// </summary>
/// <param name="_path">데이터 경로</param>
/// <param name="_resourceView">텍스쳐 데이터를 저장할 렌더 리소스 뷰</param>
void GraphicsEngine::CreateTextureDataFromDDS(std::wstring _path, ID3D11ShaderResourceView** _resourceView)
{
	HRESULT hr = S_OK;
	ID3D11Resource* texResource = nullptr;
	hr = DirectX::CreateDDSTextureFromFile(this->d3d11Device, _path.c_str(), &texResource, _resourceView);
	assert(SUCCEEDED(hr) && "cannot create resource view");
	texResource->Release();
}

void GraphicsEngine::CreateTextureDataFromTGA(std::wstring _path, ID3D11ShaderResourceView** _resourceView)
{
	HRESULT hr = S_OK;
	ID3D11Resource* texResource = nullptr;

	DirectX::ScratchImage image;
	hr = DirectX::LoadFromTGAFile(_path.c_str(), nullptr, image);
	assert(SUCCEEDED(hr) && "cannot create image when load TGA data");

	hr = DirectX::CreateTexture(this->d3d11Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texResource);
	assert(SUCCEEDED(hr) && "cannot create image when load TGA data");

	hr = this->d3d11Device->CreateShaderResourceView(texResource, nullptr, _resourceView);
	assert(SUCCEEDED(hr) && "cannot create image when load TGA data");

	texResource->Release();
}

void GraphicsEngine::CreateTextureDataFromTGA(std::vector<std::wstring> _path, ID3D11ShaderResourceView** _resourceView)
{
	HRESULT hr = S_OK;

	UINT arraySize = static_cast<UINT>(_path.size());

	D3D11_SUBRESOURCE_DATA* texture = new D3D11_SUBRESOURCE_DATA[arraySize];

	ID3D11Resource** texResource = new ID3D11Resource * [2];
	DirectX::ScratchImage* image = new DirectX::ScratchImage[2];
	for (UINT i = 0; i < arraySize; i++)
	{
		hr = DirectX::LoadFromTGAFile(_path[i].c_str(), DirectX::TGA_FLAGS_NONE, nullptr, image[i]);
		assert(SUCCEEDED(hr) && "cannot create image when load TGA data");

		hr = DirectX::CreateTexture(
			this->d3d11Device,
			image[i].GetImages(),
			image[i].GetImageCount(),
			image[i].GetMetadata(),
			&texResource[i]
		);
		assert(SUCCEEDED(hr) && "cannot create texture when load TGA data");
	}

	UINT width = static_cast<UINT>(image[0].GetImages()->width);
	UINT height = static_cast<UINT>(image[0].GetImages()->height);

	D3D11_TEXTURE2D_DESC stagingTextureDecs = {};
	D3D11_TEXTURE2D_DESC textureArrayDesc = {};

	ID3D11Texture2D** resourceTexture = new ID3D11Texture2D * [arraySize];
	ID3D11Texture2D** stagingTextureArray = new ID3D11Texture2D * [arraySize];

	for (UINT i = 0; i < arraySize; i++)
	{
		texResource[i]->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&resourceTexture[i]));

		resourceTexture[i]->GetDesc(&stagingTextureDecs);

		stagingTextureDecs.BindFlags = 0;
		stagingTextureDecs.Usage = D3D11_USAGE_STAGING;
		stagingTextureDecs.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		this->d3d11Device->CreateTexture2D(&stagingTextureDecs, nullptr, &stagingTextureArray[i]);
		assert(stagingTextureArray[i]);
		assert(resourceTexture[i]);

		this->d3d11DeviceContext->CopyResource(stagingTextureArray[i], resourceTexture[i]);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = this->d3d11DeviceContext->Map(stagingTextureArray[i], 0, D3D11_MAP_READ, 0, &mappedResource);

		texture[i].pSysMem = mappedResource.pData;
		texture[i].SysMemPitch = mappedResource.RowPitch;
		texture[i].SysMemSlicePitch = mappedResource.DepthPitch;
	}

	textureArrayDesc = stagingTextureDecs;

	textureArrayDesc.ArraySize = arraySize;
	textureArrayDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureArrayDesc.CPUAccessFlags = 0;
	textureArrayDesc.MiscFlags = 0;

	ID3D11Texture2D* textureArray;

	hr = this->d3d11Device->CreateTexture2D(&textureArrayDesc, texture, &textureArray);

	assert(textureArray);
	hr = this->d3d11Device->CreateShaderResourceView(textureArray, nullptr, _resourceView);

	textureArray->Release();
	for (UINT i = 0; i < arraySize; i++)
	{
		texResource[i]->Release();
		stagingTextureArray[i]->Release();
		resourceTexture[i]->Release();
	}

	delete[] texture;
	delete[] texResource;
	delete[] resourceTexture;
	delete[] stagingTextureArray;
}

/// <summary>
/// 텍스쳐 리소스를 픽셀 셰이더에 적용
/// </summary>
/// <param name="_start">리소스 슬릇</param>
/// <param name="_viewNumbers">리소스 갯수</param>
/// <param name="_resourceView">리소스 뷰 포인터</param>
void GraphicsEngine::SetTexture(UINT _start, UINT _viewNumbers, ID3D11ShaderResourceView** _resourceView)
{
	this->d3d11DeviceContext->PSSetShaderResources(_start, _viewNumbers, _resourceView);
}

FObject* GraphicsEngine::LoadFbxData(std::string _path)
{
	FObject* res = new FObject();
	res->fData = this->fbxLoader->Load(_path);
	return res;
}

/// <summary>
/// 인덱스 버퍼 생성
/// </summary>
/// <param name="_indices">인덱스 배열</param>
/// <param name="_size">배열의 사이즈</param>
/// <param name="_indexbuffer">버퍼를 반환받을 포인터</param>
void GraphicsEngine::CreateIndexBuffer(UINT* _indices, UINT _size, ID3D11Buffer** _indexbuffer)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * _size;
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
/// 그리기 작업을 종료하고 출력
/// </summary>
void GraphicsEngine::endDraw()
{
	this->swapChain->Present(0, 0);
}

/// <summary>
/// 그리기 작업 시작시 초기화
/// </summary>
void GraphicsEngine::begineDraw()
{
	RenderClearView();
}
