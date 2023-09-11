#pragma once
#include <d3d11.h>
#include <assert.h>
#include <vector>

#pragma comment(lib,"d3d11.lib")

/// <summary>
/// D3D �׷��� ����
/// �ۼ��� : ����ȯ
/// ���� �ۼ��� : 2023/09/06
/// ���� ������ : 2023/09/06
/// 
/// Dx11�� �̿��� 3D �׷��� ����
/// </summary>


struct Vertex
{
	float x, y, z;
	float color[4];
};

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

public:
	GraphicsEngine();
	~GraphicsEngine();

	void Initialize(HWND _hwnd);
	void RenderClearView();
	void RenderVertexLine(const std::vector<Vertex>& _vertexs);

private:
	void CreateD3D11DeviceContext();
	void CreateChainValue();
	void CreateRenderTargetView();
	void CreateDepthStencilBufferAndView();
	void BindView();

	void ClearRenderTargetView();
	void ClearDepthStencilView();
};

