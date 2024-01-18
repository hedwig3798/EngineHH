#pragma once
#include "pch.h"

struct PipeLine
{
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11ShaderResourceView** textureView = nullptr;

	UINT textureCount = 0;
	UINT vertexStructSize = 0;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	void RelasePipline()
	{
		this->inputLayout->Release();
		this->vertexShader->Release();
		this->vertexBuffer->Release();
		this->pixelShader->Release();
		this->IndexBuffer->Release();
		this->rasterizerState->Release();
		for (UINT i = 0; i < this->textureCount; i++)
		{
			this->textureView[i]->Release();
		}
	}
};
