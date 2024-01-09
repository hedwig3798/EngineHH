#pragma once
#include "pch.h"
#include "Vertex.h"
#include "color.h"
#include "pipeline.h"
#include "LightHelper.h"

/// <summary>
/// D3D �׷��� ����
/// �ۼ��� : ����ȯ
/// ���� �ۼ��� : 2023/09/06
/// ���� ������ : 2023/10/11
/// 
/// Dx11�� �̿��� 3D �׷��� ����
/// </summary>

class DXTKFont;
class RenderObject;
class FObject;
class FbxLoader;

class GraphicsEngine
{
private:

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX worldInversTranspose;
		Material material;
	};

	struct LightingBufferType
	{
		// ���籤�� (3����)
		DirectionalLight dirLights[3];
		UINT lightCount;
		// ī�޶��� ��ġ
		DirectX::XMFLOAT3 eyePosW;
		// 	float  g_fogStart;
		// 	float  g_fogRange;
		// 	float4 g_fogColor;
	};

	struct BonesBufferType
	{
		DirectX::XMMATRIX bones[100];
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
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* boneBuffer;

	bool useMSAA;

	DXTKFont* writer;
	ID3D11DepthStencilState* writerDSS;
	ID3D11RasterizerState* writerRS;

	FbxLoader* fbxLoader;

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

	void CreateInputLayer(ID3D11InputLayout** _inputLayout, D3D11_INPUT_ELEMENT_DESC* _defaultInputLayerDECS, UINT _numberOfElement, ID3D11VertexShader** _vs, std::wstring _path);
	void CreatePixelShader(ID3D11PixelShader** _ps, std::wstring _path);

	template<typename V>
	void CreateVertexBuffer(V* _verteies, UINT _size, ID3D11Buffer** _vertexbuffer);

	void CreateIndexBuffer(UINT* _indices, UINT _size, ID3D11Buffer** _indexbuffer);
	void CreateRasterizerState(ID3D11RasterizerState** _rasterizerState);

	void CreateMatrixBuffer();
	void BindMatrixParameter(DirectX::XMMATRIX _w, DirectX::XMMATRIX _v, DirectX::XMMATRIX _p, Material _material);

	void CreateBoneBuffer();
	void BindBonesData(std::vector<RenderObject*>& bones, DirectX::XMMATRIX _worldTM);

	void CreateLightingBffer();
	void BindLightingParameter(DirectionalLight _directionLight[], UINT _lightCount, DirectX::XMFLOAT3 _cameraPos);

	void BindPipeline(PipeLine& _pipline);

	void WriteText(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text);

	void CreateTextureDataFromDDS(std::wstring _path, ID3D11ShaderResourceView** _resourceView);
	void CreateTextureDataFromTGA(std::wstring _path, ID3D11ShaderResourceView** _resourceView);
	void CreateTextureDataFromTGA(std::vector<std::wstring> _path, ID3D11ShaderResourceView** _resourceView);
	void SetTexture(UINT _start, UINT _viewNumbers, ID3D11ShaderResourceView** _resourceView);

	FObject* LoadFbxData(std::string _path);

private:
	void CreateD3D11DeviceContext();
	void CreateChainValue();
	void CreateRenderTargetView();
	void CreateDepthStencilBufferAndView();
	void CreateViewport();
	void BindView();

	void CreateWriter();

	std::vector<byte> Read(std::string File);
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

