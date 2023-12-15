#include "FMesh.h"
#include "FbxData.h"
#include "FbxLoader.h"
#include "GraphicsEngine.h"

FMesh::FMesh()
	: fData(nullptr)
{

}

FMesh::~FMesh()
{

}

void FMesh::Render(GraphicsEngine* _gp, DirectX::XMMATRIX _viewTM, DirectX::XMMATRIX _projTM)
{
	_gp->BindMatrixParameter(
		DirectX::XMMatrixIdentity(),
		_viewTM,
		_projTM,
		this->demoMat
	);

	_gp->BindPipeline(this->pipeline);
	_gp->SetTexture(0, 1, &this->pipeline.textureView);
	_gp->RenderByIndex(this->pipeline, static_cast<int>(this->fData->indexData.size()));
}

void FMesh::CreatePipeline(GraphicsEngine* _gp, std::wstring _sPath[], std::wstring _texturePath)
{
	this->vertexData = new VertexF::Data[this->fData->vertexData.size()];
	memcpy(this->vertexData, &this->fData->vertexData[0], (sizeof(VertexF::Data) * this->fData->vertexData.size()));

	this->indexData = new UINT[this->fData->indexData.size()];
	memcpy(this->indexData, &this->fData->indexData[0], (sizeof(UINT) * this->fData->indexData.size()));

	_gp->CreateInputLayer(this->pipeline, VertexF::defaultInputLayerDECS, _sPath, 5);
	_gp->CreateVertexBuffer(this->vertexData, static_cast<UINT>(this->fData->vertexData.size()) * VertexF::Size(), &this->pipeline.vertexBuffer);
	_gp->CreateIndexBuffer(this->indexData, static_cast<UINT>(this->fData->indexData.size()), &this->pipeline.IndexBuffer);
	_gp->CreateRasterizerState(&this->pipeline.rasterizerState);
	pipeline.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline.vertexStructSize = VertexF::Size();
}

