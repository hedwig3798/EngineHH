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
	// �ϴ� �ϵ��ڵ����� �� �ִ� ������ 100���� ����
	// �������� ��� ���۸� ���� ����� ������?
    matrix bones[1000];
};

VertexOut VS(VertexIn vin)
{
	// ���
	VertexOut vout;

	// ������ ������� ��ǥ
	vout.PosW = mul(float4(vin.PosL, 1.0f), g_world).xyz;
	// ������ ����Ʈ ��ǥ
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_wvp);

	// �ؽ����� ��ǥ (texcoord�� �״�� �ȼ����̴��� �Ѱ� �ش�)
	vout.Tex = vin.Tex;

	//������ ���� �������� ���� ����
	vout.NormalW = normalize(mul(vin.NormalL, (float3x3)g_worldInvTranspose));
	vout.Tangent = normalize(mul(vin.Tangent, (float3x3)g_worldInvTranspose));
	vout.Binormal = normalize(mul(vin.Binormal, (float3x3)g_worldInvTranspose));
	return vout;
}