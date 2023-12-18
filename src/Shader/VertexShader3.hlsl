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
	DirectionalLight g_dirLights[3];
	uniform int g_lightCount;
	float3 g_eyePosW;
};


struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
    float3 Weight : BINORMAL;
    float3 BoneIndices : TANGENT;
};

struct VOUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VOUT VS(VertexIn _vin)
{
	// Ãâ·Â
	VOUT vout;

	vout.pos = float4(_vin.PosL, 1.0f);
    vout.pos = mul(vout.pos, g_wvp);
    vout.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return vout;
}