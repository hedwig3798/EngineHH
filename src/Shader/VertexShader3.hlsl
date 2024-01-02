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
	unsigned int texindex : TEXINDEX;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
    float3 NormalW : NORMAL;
	Material material : MATERIAL;
	unsigned int texindex : TEXINDEX;
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
	vout.NormalW = mul(vin.NormalL, (float3x3)g_worldInvTranspose);
	vout.material = g_material;
	vout.texindex = vin.texindex;
	return vout;
}