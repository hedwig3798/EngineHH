#include "../Shader/LightHelper.hlsl"

cbuffer cbPerObject : register(b0)
{
    matrix g_world;
    matrix g_wvp;
    matrix g_worldInvTranspose;
	Material g_material;
};

cbuffer cbPerFrame : register(b1)
{
	// 직사광선 (3종류)
	DirectionalLight g_dirLights[3];
	uniform int g_lightCount;
	// 카메라의 위치
	float3 g_eyePosW;
// 	float  g_fogStart;
// 	float  g_fogRange;
// 	float4 g_fogColor;
};

cbuffer cbPerObject : register(b2)
{
	// 일단 하드코딩으로 본 최대 갯수를 100개로 설정
	// 유동적인 상수 버퍼를 만들 방법이 없을까?
    matrix bones[100];
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
    float3 Weight : WEIGHT;
    uint4 BoneIndices : BONEINDICES;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
    float3 NormalW : NORMAL;
	Material material : MATERIAL;
};

VertexOut VS(VertexIn vin)
{
	// 출력
	VertexOut vout;
	
	// 정점의 월드공간 좌표
	vout.PosW = mul(float4(vin.PosL, 1.0f), g_world).xyz;
	// 정점의 뷰포트 좌표
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_wvp);

	// 텍스쳐의 좌표 (texcoord를 그대로 픽셀쉐이더로 넘겨 준다)
	vout.Tex = vin.Tex;

	//정점의 월드 공간에서 법선 벡터
	vout.NormalW = mul(vin.NormalL, (float3x3)g_worldInvTranspose);
	// vout.NormalW = vin.NormalL;

	vout.material = g_material;
	return vout;
}