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
			, texIndex(0)
		{}

		Data(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _normal, DirectX::XMFLOAT2 _UV, DirectX::XMFLOAT3 _binormal, DirectX::XMFLOAT3 _tangent, UINT _texIndex)
			: position(_pos)
			, normal(_normal)
			, UV(_UV)
			, binormal(_binormal)
			, tangent(_tangent)
			, texIndex(_texIndex)
		{
		}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 binormal;
		DirectX::XMFLOAT3 tangent;
		UINT texIndex;

		// unordered_map에서 사용될 때 해쉬 충돌을 처리해 줄 연산자 오버로딩
		bool operator==(const Data& _other) const
		{
			return !(memcmp(this, &_other, sizeof(Data)));
		}
		// unordered_map에서 사용할 해쉬 함수 ( 모든 값을 해쉬화 하고 XOR 연산함 )
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

	static D3D11_INPUT_ELEMENT_DESC defaultInputLayerDECS[6] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}