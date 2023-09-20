#pragma once
#include "pch.h"
#include "Vertex.h"
#include "color.h"
#include "pipeline.h"


/// <summary>
/// D3D 그래픽 엔진
/// 작성자 : 김형환
/// 최초 작성일 : 2023/09/06
/// 최종 수정일 : 2023/09/18
/// 
/// Dx11을 이용한 3D 그래픽 엔진
/// </summary>

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
	UINT createDeviceFlags = 0;

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

public:
	GraphicsEngine();
	~GraphicsEngine();

	void Initialize(HWND _hwnd);
	void RenderClearView();
	void RenderTestThing(PipeLine& _pipline);
	void endDraw();
	void begineDraw();

	void ClearRenderTargetView();
	void ClearDepthStencilView();

	void CreateInputLayer(ID3D11InputLayout* _inputLayout, ID3D11VertexShader* _vertexShader, ID3D11PixelShader* _pixelShader);
	void CreateVertexBuffer(Vertex* _verteies, size_t _size, ID3D11Buffer* _vertexbuffer);
	void CreateIndexBuffer(UINT* _indices, size_t _size, ID3D11Buffer* _indexbuffer);
	void CreateRasterizerState(ID3D11RasterizerState* _rasterizerState);

	void SetParameter(DirectX::XMMATRIX _w, DirectX::XMMATRIX _v, DirectX::XMMATRIX _p);

private:
	void CreateD3D11DeviceContext();
	void CreateChainValue();
	void CreateRenderTargetView();
	void CreateDepthStencilBufferAndView();
	void BindView();

};

