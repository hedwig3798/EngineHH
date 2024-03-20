#include "GeometryGenerator.h"
#include "pipeline.h"
#include "GraphicsEngine.h"

PipeLine GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, GraphicsEngine* _gp)
{

	std::vector<Vertex> vData;
	std::vector<UINT> iData;

	Vertex topVertex;
	topVertex.Position = { 0.0f, +radius, 0.0f };
	topVertex.UV = { 0.0f, 0.0f };

	Vertex bottomVertex;
	bottomVertex.Position = { 0.0f, -radius, 0.0f };
	bottomVertex.UV = { 0.0f, 1.0f };

	vData.push_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = radius * sinf(phi) * cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi) * sinf(theta);

			v.UV.x = theta / DirectX::XM_2PI;
			v.UV.y = phi / DirectX::XM_PI;

			vData.push_back(v);
		}
	}

	vData.push_back(bottomVertex);

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		iData.push_back(0);
		iData.push_back(i + 1);
		iData.push_back(i);
	}

	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			iData.push_back(baseIndex + i * ringVertexCount + j);
			iData.push_back(baseIndex + i * ringVertexCount + j + 1);
			iData.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			iData.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			iData.push_back(baseIndex + i * ringVertexCount + j + 1);
			iData.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)vData.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		iData.push_back(southPoleIndex);
		iData.push_back(baseIndex + i);
		iData.push_back(baseIndex + i + 1);
	}

	PipeLine result;
	_gp->CreateVertexBuffer(vData.data(), vData.size() * sizeof(Vertex), result.vertexBuffer, "sphere");
	_gp->CreateIndexBuffer(iData.data(), iData.size(), result.IndexBuffer);

	return result;
}
