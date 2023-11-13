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
	// ���籤�� (3����)
	DirectionalLight g_dirLights[3];
	uniform int g_lightCount;
	// ī�޶��� ��ġ
	float3 g_eyePosW;
// 	float  g_fogStart;
// 	float  g_fogRange;
// 	float4 g_fogColor;
};

cbuffer cbPerObject : register(b2)
{
	// �ϴ� �ϵ��ڵ����� �� �ִ� ������ 100���� ����
	// �������� ��� ���۸� ���� ����� ������?
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
	// ���
	VertexOut vout;
	
	float weight[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	weight[0] = vin.Weight.x;
	weight[1] = vin.Weight.y;
	weight[2] = vin.Weight.z;
	weight[3] = vin.Weight.x + vin.Weight.y + vin.Weight.z - 1.0f;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for(int i = 0 ; i < 4 ; i++)
	{
		posL += weight[i] * mul(float4(vin.PosL, 1.0f), bones[vin.BoneIndices[i]]).xyz;
		normalL += weight[i] * mul(vin.NormalL, (float3x3)bones[vin.BoneIndices[i]]);
	}

	// ������ ������� ��ǥ
	vout.PosW = mul(float4(posL, 1.0f), g_world).xyz;
	// ������ ����Ʈ ��ǥ
	vout.PosH = mul(float4(posL, 1.0f), g_wvp);

	// �ؽ����� ��ǥ (texcoord�� �״�� �ȼ����̴��� �Ѱ� �ش�)
	vout.Tex = vin.Tex;

	//������ ���� �������� ���� ����
	vout.NormalW = mul(normalL, (float3x3)g_worldInvTranspose);
	// vout.NormalW = vin.NormalL;
	
	

	vout.material = g_material;
	return vout;
}