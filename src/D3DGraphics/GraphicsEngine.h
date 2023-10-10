#pragma once
#include "pch.h"
#include "Vertex.h"
#include "color.h"
#include "pipeline.h"

/// <summary>
/// D3D 그래픽 엔진
/// 작성자 : 김형환
/// 최초 작성일 : 2023/09/06
/// 최종 수정일 : 2023/10/10
/// 
/// Dx11을 이용한 3D 그래픽 엔진
/// </summary>

class DXTKFont;

class GraphicsEngine
{
private:

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	// D3D 기능 레벨
	D3D_FEATURE_LEVEL featureLevel;

	// D3D 디바이스와 디바이스 컨텍스트
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DeviceContext;

	// 디바이스 플래그
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

	// 윈도우 핸들러
	HWND hwnd;

	// 스왑 체인
	IDXGISwapChain* swapChain;

	// 렌더 타겟 뷰
	ID3D11RenderTargetView* renderTargetView;

	// m4xMass 수준
	UINT m4xMsaaQuality;

	ID3D11Texture2D* depthStancilBuffer;
	ID3D11DepthStencilView* depthStancilView;

	ID3D11Buffer* matrixBuffer;

	bool useMSAA;

	DXTKFont* writer;
	ID3D11DepthStencilState* writerDSS;
	ID3D11RasterizerState* writerRS;

public:
	GraphicsEngine();
	~GraphicsEngine();

	void Initialize(HWND _hwnd);
	void RenderClearView();
	void RenderTestThing(PipeLine& _pipline);
	void RenderByIndex(PipeLine& _pipline, int _indexSize);
	void endDraw();
	void begineDraw();

	void ClearRenderTargetView();
	void ClearDepthStencilView();

	void CreateInputLayer(PipeLine& _pipline, D3D11_INPUT_ELEMENT_DESC* _defaultInputLayerDECS, std::wstring _path[], UINT _numberOfElement);

	template<typename V>
	void CreateVertexBuffer(V* _verteies, UINT _size, ID3D11Buffer** _vertexbuffer);

	void CreateIndexBuffer(UINT* _indices, UINT _size, ID3D11Buffer** _indexbuffer);
	void CreateRasterizerState(ID3D11RasterizerState** _rasterizerState);

	void SetParameter(DirectX::XMMATRIX _w, DirectX::XMMATRIX _v, DirectX::XMMATRIX _p);

	void BindPipeline(PipeLine& _pipline);

	void WriteText(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text);

	void CreateTextureData(std::wstring _path, ID3D11ShaderResourceView** _resourceView);
	void SetTexture(UINT _start, UINT _viewNumbers, ID3D11ShaderResourceView** _resourceView);
private:
	void CreateD3D11DeviceContext();
	void CreateChainValue();
	void CreateRenderTargetView();
	void CreateDepthStencilBufferAndView();
	void CreateViewport();
	void BindView();

	void CreateWriter();
};

template<typename V>
void GraphicsEngine::CreateVertexBuffer(V* _verteies, UINT _size, ID3D11Buffer** _vertexbuffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vb = {};

	vb.Usage = D3D11_USAGE_IMMUTABLE;
	vb.ByteWidth = _size;
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

