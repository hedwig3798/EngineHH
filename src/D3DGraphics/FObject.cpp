#include "FObject.h"
#include "FbxData.h"
#include "FbxLoader.h"
#include "GraphicsEngine.h"
#include "fbxVertex.h"
#include "FbxMeshData.h"

FObject::FObject()
	: fData(nullptr)
	, demoMat{}
{
	this->demoMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->demoMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	this->demoMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

FObject::~FObject()
{

}

void FObject::Render(GraphicsEngine* _gp, DirectX::XMMATRIX _viewTM, DirectX::XMMATRIX _projTM)
{
	FbxMeshData* _nowData = this->fData->mesh;
	_gp->BindDeferredView();
	_gp->DeferredRenderClearView();
	for (auto& c : _nowData->children)
	{
		_gp->BindMatrixParameter(
			c->globalTM,
			_viewTM,
			_projTM,
			this->demoMat
		);
		if (c->vertexData.size() != 0)
		{
			_gp->BindPipeline(*c->pipeline);
			_gp->SetTexture(0, 1, c->pipeline->textureView);
			_gp->RenderByIndex(*c->pipeline, static_cast<int>(c->indexData.size()));
		}
	}
}

void FObject::Initalize(GraphicsEngine* _gp, std::wstring _sPath[], std::wstring _texturePath, FbxData* _nowData)
{
	this->gp = _gp;
	this->path = _sPath;
	this->texturePath = _texturePath;
	CreatePipeline(this->fData->mesh);
}

void FObject::CreatePipeline(FbxMeshData* _nowMesh)
{
	if (!_nowMesh->vertexData.size() == 0)
	{
		_nowMesh->pipeline = new PipeLine();
		this->gp->CreateInputLayer(&_nowMesh->pipeline->inputLayout, VertexF::defaultInputLayerDECS, 6, &_nowMesh->pipeline->vertexShader, this->path[0]);
		this->gp->CreatePixelShader(&_nowMesh->pipeline->pixelShader, this->path[1]);
		this->gp->CreateRasterizerState(&_nowMesh->pipeline->rasterizerState);
		_nowMesh->pipeline->primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_nowMesh->pipeline->vertexStructSize = VertexF::Size();


		std::vector<std::wstring> path(_nowMesh->textureFileName.size());
		for (int i = 0; i < _nowMesh->textureFileName.size(); i++)
		{
			path[i].assign(_nowMesh->textureFileName[i].begin(), _nowMesh->textureFileName[i].end());
			path[i] = this->texturePath + path[i];
		}

		if (!path.empty())
		{
			_nowMesh->pipeline->textureView = new ID3D11ShaderResourceView * [path.size()];
			this->gp->CreateTextureDataFromTGA(path, _nowMesh->pipeline->textureView);
		}

		_nowMesh->vertexBufferData = new VertexF::Data[_nowMesh->vertexData.size()];
		memcpy(_nowMesh->vertexBufferData, &_nowMesh->vertexData[0], (sizeof(VertexF::Data) * _nowMesh->vertexData.size()));
		this->gp->CreateVertexBuffer(_nowMesh->vertexBufferData, static_cast<UINT>(_nowMesh->vertexData.size()) * VertexF::Size(), &_nowMesh->pipeline->vertexBuffer);

		_nowMesh->indexBufferData = new UINT[_nowMesh->indexData.size()];
		memcpy(_nowMesh->indexBufferData, &_nowMesh->indexData[0], (sizeof(UINT) * _nowMesh->indexData.size()));

		this->gp->CreateIndexBuffer(_nowMesh->indexBufferData, static_cast<UINT>(_nowMesh->indexData.size()), &_nowMesh->pipeline->IndexBuffer);
	}

	for (auto& c : _nowMesh->children)
	{
		CreatePipeline(c);
	}
}
