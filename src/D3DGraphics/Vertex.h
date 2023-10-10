#pragma once
#include "pch.h"

/// <summary>
/// 정점 구조체
/// </summary>
namespace VertexC
{
	struct Data
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	static UINT Size()
	{
		return (UINT)sizeof(Data);
	}

	static D3D11_INPUT_ELEMENT_DESC defaultInputLayerDECS[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}

namespace VertexT
{
	struct Data
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	static UINT Size()
	{
		return (UINT)sizeof(Data);
	}

	static D3D11_INPUT_ELEMENT_DESC defaultInputLayerDECS[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}
