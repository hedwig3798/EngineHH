#include "FObject.h"
#include "FbxData.h"
#include "FbxLoader.h"
#include "GraphicsEngine.h"
#include "IGraphicsEngine.h"
#include "fbxVertex.h"
#include "FbxMeshData.h"
#include "LightHelper.h"
#include "pipeline.h"

FObject::FObject()
	: fData(nullptr)
	, path(nullptr)
	, gp(nullptr)
{
}

FObject::~FObject()
{
	ReleasePipeline(this->fData->mesh);
	delete this->fData;
}

void FObject::Render()
{
	FbxMeshData* _nowData = this->fData->mesh;
	for (auto& c : _nowData->children)
	{
		gp->BindMatrixParameter(c->globalTM);
		if (c->vertexData.size() != 0)
		{
			gp->BindPipeline(*c->pipeline);
			gp->SetTexture(0, 1, c->pipeline->textureView);
			gp->Render(*c->pipeline, static_cast<int>(c->indexData.size()));
		}
	}
}

void FObject::Initalize(IGraphicsEngine* _gp, std::wstring _sPath[], std::wstring _texturePath, FbxData* _nowData)
{
	this->gp = dynamic_cast<GraphicsEngine*>(_gp);
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

void FObject::ReleasePipeline(FbxMeshData* _nowMesh)
{
	if (_nowMesh->pipeline)
	{
		_nowMesh->pipeline->RelasePipline();
		delete _nowMesh->pipeline;
	}
	for (auto& c : _nowMesh->children)
	{
		ReleasePipeline(c);
	}
}
