#include "GeometryGenerator.h"
#include "pipeline.h"
#include "GraphicsEngine.h"

/// <summary>
/// 구 모델을 만드는 함수
/// </summary>
/// <param name="radius">지름</param>
/// <param name="sliceCount">가로 슬라이스 크기</param>
/// <param name="stackCount">세로 슬라이스 크기</param>
/// <param name="_gp">그래픽 엔진</param>
/// <returns>정점과 인덱스가 있는 파이프라인</returns>
PipeLine GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, GraphicsEngine* _gp)
{
	// 데이터를 저장 할 벡터
	std::vector<Vertex> vData;
	std::vector<UINT> iData;

	// 위 꼭지점과 아래 꼭지점
	Vertex topVertex;
	topVertex.Position = { 0.0f, +radius, 0.0f };
	topVertex.UV = { 0.0f, 0.0f };

	Vertex bottomVertex;
	bottomVertex.Position = { 0.0f, -radius, 0.0f };
	bottomVertex.UV = { 0.0f, 1.0f };

	// 위 꼭지점 부터 시작한다.

	vData.push_back(topVertex);
	// y 축을 기준으로 나눴을 때 각도
	float phiStep = DirectX::XM_PI / stackCount;
	// 한 줄에 만들어질 정점들 사이의 각도
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	// 반복 시작
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		// 가로줄 각도
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			// 세로줄 각도
			float theta = j * thetaStep;

			Vertex v;

			// 계산하기
			// 구를 위에서 보고 계산한다고 생각하면 편하다
			v.Position.x = radius * sinf(phi) * cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi) * sinf(theta);

			v.UV.x = theta / DirectX::XM_2PI;
			v.UV.y = phi / DirectX::XM_PI;

			vData.push_back(v);
		}
	}

	// 아래 꼭지점 입력
	vData.push_back(bottomVertex);

	// 인덱스 넣기
	
	// 윗 꼭지점과 이어진 삼각형들
	for (UINT i = 1; i <= sliceCount; ++i)
	{
		iData.push_back(0);
		iData.push_back(i + 1);
		iData.push_back(i);
	}

	// 기본 값
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

	UINT southPoleIndex = (UINT)vData.size() - 1;


	baseIndex = southPoleIndex - ringVertexCount;

	// 아래 꼭지점과 이어진 삼각형들
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
