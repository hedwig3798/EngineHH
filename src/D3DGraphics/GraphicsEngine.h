#pragma once
#include "pch.h"
#include "Vertex.h"
#include "color.h"
#include "pipeline.h"

/// <summary>
/// D3D �׷��� ����
/// �ۼ��� : ����ȯ
/// ���� �ۼ��� : 2023/09/06
/// ���� ������ : 2023/09/25
/// 
/// Dx11�� �̿��� 3D �׷��� ����
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

	// D3D ��� ����
	D3D_FEATURE_LEVEL featureLevel;

	// D3D ����̽��� ����̽� ���ؽ�Ʈ
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DeviceContext;

	// ����̽� �÷���
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

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

	void CreateInputLayer(ID3D11InputLayout** _inputLayout, ID3D11VertexShader** _vertexShader, ID3D11PixelShader** _pixelShader);
	void CreateVertexBuffer(Vertex* _verteies, size_t _size, ID3D11Buffer** _vertexbuffer);
	void CreateIndexBuffer(UINT* _indices, size_t _size, ID3D11Buffer** _indexbuffer);
	void CreateRasterizerState(ID3D11RasterizerState** _rasterizerState);

	void SetParameter(DirectX::XMMATRIX _w, DirectX::XMMATRIX _v, DirectX::XMMATRIX _p);

	void BindPipeline(PipeLine& _pipline);

	void WriteText(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text);

private:
	void CreateD3D11DeviceContext();
	void CreateChainValue();
	void CreateRenderTargetView();
	void CreateDepthStencilBufferAndView();
	void CreateViewport();
	void BindView();

	void CreateWriter();
};

