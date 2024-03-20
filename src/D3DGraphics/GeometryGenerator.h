#pragma once
#include "pch.h"

class PipeLine;
class GraphicsEngine;

namespace GeometryGenerator
{
	struct Vertex
	{
		Vertex() = default;
		Vertex(const Vertex&) = default;
		Vertex& operator=(const Vertex&) = default;
		Vertex(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 normal, DirectX::SimpleMath::Vector3 tangent, DirectX::SimpleMath::Vector2 texture)
			: Position(position)
			, UV(texture)
		{
		}
		Vertex(float px, float py, float pz, float nx, float ny, float nz, float tanx, float tany, float tanz, float tx, float ty)
			: Position(px, py, pz)
			, UV(tx, ty)
		{
		}

		DirectX::SimpleMath::Vector3 Position;
		DirectX::SimpleMath::Vector2 UV;
	};

	PipeLine CreateSphere(float radius, UINT sliceCount, UINT stackCount, GraphicsEngine* _gp);
};

