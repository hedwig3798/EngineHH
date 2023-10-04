#pragma once
#include "pch.h"

struct PipeLine
{
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* IndexBuffer;
	ID3D11RasterizerState* rasterizerState;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

	void relasePipline()
	{
		inputLayout->Release();
		vertexShader->Release();
		vertexBuffer->Release();
		pixelShader->Release();
		IndexBuffer->Release();
		rasterizerState->Release();
	}
};