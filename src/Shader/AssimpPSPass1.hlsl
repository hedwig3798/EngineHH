#include "../Shader/AssimpVS.hlsl"


Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metailicMap : register(t2);
Texture2D roughnessMap : register(t3);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct DeferredOutput
{
	float4 texDiffuse : SV_Target0;
	float4 texNormal : SV_Target1;
	float4 zDepth : SV_Target2;
	float4 worldPosition : SV_Target3;
	float4 metailic : SV_Target4;
	float4 roughness : SV_Target5;
};

DeferredOutput PS(VertexOut pin)
{
	DeferredOutput output;

	// diffuse
	float4 diffuse = diffuseMap.Sample(samAnisotropic, pin.Tex);
	output.texDiffuse = diffuse;


	// normal
	float3 mappedNormal = normalMap.Sample(samAnisotropic, pin.Tex).xyz;
	mappedNormal = mappedNormal * 2.0f - 1.0f;

    float3 finalNormal;
	output.texNormal = float4(pin.NormalW, 1.0f);

	// metalic
	float4 mappedmetailic = metailicMap.Sample(samAnisotropic, pin.Tex).x;
	mappedmetailic.w = 1.0f;
	output.metailic = mappedmetailic;

	
	// NDC 
	float4 worldPos = float4(pin.PosW.xyz, 1.0f);
	output.worldPosition = worldPos;
	

	// roughness
	float4 mappedroughness = roughnessMap.Sample(samAnisotropic, pin.Tex).x;
	mappedroughness.w = 1.0f;
	output.roughness = mappedroughness;


	// depth
	float d = pin.PosH.z / pin.PosH.w;
	output.zDepth = float4(d, d, d, 0);

    return output;
}