#pragma once
#include "pch.h"
#include "Vertex.h"
#include "color.h"


/// <summary>
/// D3D �׷��� ����
/// �ۼ��� : ����ȯ
/// ���� �ۼ��� : 2023/09/06
/// ���� ������ : 2023/09/18
/// 
/// Dx11�� �̿��� 3D �׷��� ����
/// </summary>

class GraphicsEngine
{
private:
	// D3D ��� ����
	D3D_FEATURE_LEVEL featureLevel;

	// D3D ����̽��� ����̽� ���ؽ�Ʈ
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DeviceContext;

	// ����̽� �÷���
	UINT createDeviceFlags = 0;

	// ������ �ڵ鷯
	HWND hwnd;

	// ���� ü��
	IDXGISwapChain* swapChain;

	// ���� Ÿ�� ��
	ID3D11RenderTargetView* renderTargetView;

	// m4xMass ����
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

