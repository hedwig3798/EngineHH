#pragma once
#include "pch.h"
#include "Vertex.h"
#include "color.h"


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

	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* defaultVs;
	ID3D11PixelShader* defaultPs;

	Vertex tempVertex[8] =
	{
		{DirectX::XMFLOAT3{-1.0f, -1.0f, -1.0f}, COLORS::White},
		{DirectX::XMFLOAT3{-1.0f, 1.0f, -1.0f}, COLORS::Black},
		{DirectX::XMFLOAT3{1.0f, 1.0f, -1.0f}, COLORS::Red},
		{DirectX::XMFLOAT3{1.0f, -1.0f, -1.0f}, COLORS::Green},
		{DirectX::XMFLOAT3{-1.0f, -1.0f, 1.0f}, COLORS::Blue},
		{DirectX::XMFLOAT3{-1.0f, 1.0f, 1.0f}, COLORS::Yellow},
		{DirectX::XMFLOAT3{1.0f, 1.0f, 1.0f}, COLORS::Cyan},
		{DirectX::XMFLOAT3{1.0f, -1.0f, 1.0f}, COLORS::Magenta},
	};

	std::vector<ID3D11Buffer*> vertexBuffers;

public:
	GraphicsEngine();
	~GraphicsEngine();

	void Initialize(HWND _hwnd);
	void RenderClearView();
	void RenderVertexLine(const std::vector<Vertex>& _vertexs);
	void RenderTestThing();

private:
	void CreateD3D11DeviceContext();
	void CreateChainValue();
	void CreateRenderTargetView();
	void CreateDepthStencilBufferAndView();
	void BindView();
	void CreateInputLayer();

	void ClearRenderTargetView();
	void ClearDepthStencilView();
	void InputVertexBuffer(Vertex* _verteies, size_t _size);
};

