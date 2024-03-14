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
	unsigned int texindex : TEXINDEX;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
    float3 NormalW : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
	unsigned int texindex : TEXINDEX;
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
	vout.NormalW = normalize(mul(vin.NormalL, (float3x3)g_worldInvTranspose));
	vout.Tangent = normalize(mul(vin.Tangent, (float3x3)g_worldInvTranspose));
	vout.Binormal = normalize(mul(vin.Binormal, (float3x3)g_worldInvTranspose));
	vout.texindex = vin.texindex;
	return vout;
}