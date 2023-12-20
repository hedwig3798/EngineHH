#pragma once
#include "pch.h"

namespace VertexF
{
	struct Data
	{
		Data() 
			: position()
			, normal()
			, UV()
			, binormal()
			, tangent() 
		{}

		Data(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _normal, DirectX::XMFLOAT2 _UV, DirectX::XMFLOAT3 _binormal, DirectX::XMFLOAT3 _tangent)
			: position(_pos)
			, normal(_normal)
			, UV(_UV)
			, binormal(_binormal)
			, tangent(_tangent)
		{
		}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 binormal;
		DirectX::XMFLOAT3 tangent;

		bool operator==(const Data& _other) const
		{
			if (!(position.x == _other.position.x &&
				position.y == _other.position.y &&
				position.z == _other.position.z))
			{
				return false;
			}
			if (!(normal.x == _other.normal.x &&
				normal.y == _other.normal.y &&
				normal.z == _other.normal.z))
			{
				return false;
			}
			if (!(binormal.x == _other.binormal.x &&
				binormal.y == _other.binormal.y &&
				binormal.z == _other.binormal.z))
			{
				return false;
			}
			if (!(tangent.x == _other.tangent.x &&
				tangent.y == _other.tangent.y &&
				tangent.z == _other.tangent.z))
			{
				return false;
			}
			if (!(UV.x == _other.UV.x &&
				UV.y == _other.UV.y))
			{
				return false;
			}
			return true;
		}

		std::size_t operator() (const Data& _data) const
		{
			std::size_t h1 = std::hash<float>()(_data.position.x);
			h1 ^= std::hash<float>()(_data.position.y);
			h1 ^= std::hash<float>()(_data.position.z);

			h1 ^= std::hash<float>()(_data.normal.x);
			h1 ^= std::hash<float>()(_data.normal.y);
			h1 ^= std::hash<float>()(_data.normal.z);

			h1 ^= std::hash<float>()(_data.binormal.x);
			h1 ^= std::hash<float>()(_data.binormal.y);
			h1 ^= std::hash<float>()(_data.binormal.z);

			h1 ^= std::hash<float>()(_data.tangent.x);
			h1 ^= std::hash<float>()(_data.tangent.y);
			h1 ^= std::hash<float>()(_data.tangent.z);

			h1 ^= std::hash<float>()(_data.UV.x);
			h1 ^= std::hash<float>()(_data.UV.y);
			return h1;
		}
	};

	static UINT Size()
	{
		return static_cast<UINT>(sizeof(Data));
	}

	static D3D11_INPUT_ELEMENT_DESC defaultInputLayerDECS[5] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}