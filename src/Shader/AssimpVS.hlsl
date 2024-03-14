#include "../Shader/LightHelper.hlsl"

cbuffer cbPerObject : register(b0)
{
    matrix g_world;
    matrix g_wvp;
    matrix g_worldInvTranspose;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
    float3 Binormal : BINORMAL;
    float3 Tangent : TANGENT;
	int4 bIndex : BINDEX;
	float4 bWeight : BWEIGHT;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
    float3 NormalW : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

cbuffer cbPerObject : register(b1)
{
	// 일단 하드코딩으로 본 최대 갯수를 100개로 설정
	// 유동적인 상수 버퍼를 만들 방법이 없을까?
    matrix bones[1000];
};

VertexOut VS(VertexIn vin)
{
	// 출력
	VertexOut vout;
	
	float weightSum = 0.0f;
	
	float weight[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	weight[0] = vin.bWeight.x;
	weight[1] = vin.bWeight.y;
	weight[2] = vin.bWeight.z;
	weight[3] = vin.bWeight.w;

	weightSum = weight[0] + weight[1] + weight[2] + weight[3];

	unsigned int bIndex[4] = {0, 0, 0, 0};
	bIndex[0] = vin.bIndex.x;
	bIndex[1] = vin.bIndex.y;
	bIndex[2] = vin.bIndex.z;
	bIndex[3] = vin.bIndex.w;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	if (weightSum <= 0.0f)
	{
		posL = float4(vin.PosL, 1.0f);
		normalL = vin.NormalL;
	}
	else
	{
		for(int i = 0 ; i < 4 ; i++)
		{
			posL += weight[i] * mul(float4(vin.PosL, 1.0f), bones[bIndex[i]]);
			normalL += weight[i] * mul(vin.NormalL, (float3x3)bones[bIndex[i]]);
		}
	}


	// 정점의 월드공간 좌표
	vout.PosW = mul(float4(posL, 1.0f), g_world).xyz;
	// 정점의 뷰포트 좌표
	vout.PosH = mul(float4(posL, 1.0f), g_wvp);

	// 텍스쳐의 좌표 (texcoord를 그대로 픽셀쉐이더로 넘겨 준다)
	vout.Tex = vin.Tex;

	//정점의 월드 공간에서 법선 벡터
	vout.NormalW = normalize(mul(vin.NormalL, (float3x3)g_worldInvTranspose));
	vout.Tangent = normalize(mul(vin.Tangent, (float3x3)g_worldInvTranspose));
	vout.Binormal = normalize(mul(vin.Binormal, (float3x3)g_worldInvTranspose));
	return vout;
}