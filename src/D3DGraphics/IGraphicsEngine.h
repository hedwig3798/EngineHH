#pragma once
#include <windows.h>
#include <vector>
#include <string>


#pragma region ID3D11Class
class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;
class ID3D11Texture2D;
class ID3D11DepthStencilView;
class ID3D11Buffer;
class DXTKFont;
class ID3D11DepthStencilState;
class ID3D11RasterizerState;
class ID3D11ShaderResourceView;
class ID3D11InputLayout;
class D3D11_INPUT_ELEMENT_DESC;
class ID3D11PixelShader;
class ID3D11VertexShader;
#pragma endregion
class PipeLine;
class FObject;

class IGraphicsEngine abstract
{
public:
	// D3D 디바이스와 디바이스 컨텍스트
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DeviceContext;

	// 윈도우 핸들러
	HWND hwnd;

	UINT windowWidth;
	UINT windowHeight;

	// 스왑 체인
	IDXGISwapChain* swapChain;

	// 렌더 타겟 뷰
	ID3D11RenderTargetView* renderTargetView;

	// m4xMass 수준
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

	/*	FbxLoader* fbxLoader;*/

	// 	std::vector<ID3D11Texture2D*> dTexture;
	// 	std::vector<ID3D11ShaderResourceView*> dSRV;
	// 	std::vector<ID3D11RenderTargetView*> dRenderTargets;

	//	PipeLine* DPipeline;
	// 	VertexD::Data DVdata[4];
	// 	UINT DIdata[6];

public:
	virtual ~IGraphicsEngine() {};

	virtual void Initialize(HWND _hwnd) abstract;

#pragma region Render
	virtual void begineDraw() abstract;
	virtual void Render(PipeLine* _pipline, int _indexSize) abstract;
	virtual void endDraw() abstract;
#pragma endregion

#pragma region CreatePipeline
	virtual void CreateInputLayer(ID3D11InputLayout** _inputLayout, D3D11_INPUT_ELEMENT_DESC* _defaultInputLayerDECS, UINT _numberOfElement, ID3D11VertexShader** _vs, std::wstring _path) abstract;
	virtual void CreatePixelShader(ID3D11PixelShader** _ps, std::wstring _path) abstract;
// 	template<typename V>
// 	void CreateVertexBuffer(V* _verteies, UINT _size, ID3D11Buffer** _vertexbuffer);
	virtual void CreateIndexBuffer(UINT* _indices, UINT _size, ID3D11Buffer** _indexbuffer) abstract;
	virtual void CreateRasterizerState(ID3D11RasterizerState** _rasterizerState) abstract;

	virtual void BindPipeline(PipeLine& _pipline) abstract;
#pragma endregion

	virtual void WriteText(int _x, int _y, float _rgba[4], TCHAR* _text) abstract;

	virtual void LoadResourceViewFormFile(std::wstring _path, ID3D11ShaderResourceView** _resourceView) abstract;
	virtual void LoadResourceViewArrayFormFile(std::vector<std::wstring> _path, ID3D11ShaderResourceView** _resourceView) abstract;

	virtual FObject* LoadFbxData(std::string _path) abstract;
};