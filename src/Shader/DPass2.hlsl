#include "../Shader/DPass2VS.hlsl"
#include "LightHelper.hlsl"

Texture2D diffuseMaps[3] : register(t0);

cbuffer cbPerFrame : register(b0)
{
	DirectionalLight g_dirLights;
	float3 g_eyePosW;
};

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
};

float4 PS(VOUT pin) : SV_Target
{
	DirectionalLight dirLights[3];

	dirLights[0].Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[0].Diffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLights[0].Specular = float4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLights[0].Direction = float3(0.57735f, -0.57735f, 0.57735f);

	dirLights[1].Ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[1].Diffuse = float4(0.20f, 0.20f, 0.20f, 1.0f);
	dirLights[1].Specular = float4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Direction = float3(-0.57735f, -0.57735f, 0.57735f);

	dirLights[2].Ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Diffuse = float4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[2].Specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Direction = float3(0.0f, -0.707f, -0.707f);

	float4 textureColor = diffuseMaps[0].Sample(samAnisotropic, pin.Tex);
	float4 normal = diffuseMaps[1].Sample(samAnisotropic, pin.Tex);
	float4 depth = diffuseMaps[2].Sample(samAnisotropic, pin.Tex);


	float4 output;
	float4 temp;	
	float ndl = dot( normal.xyz, (dirLights[0].Direction) );
	temp = max(float4(0.0f, 0.0f, 0.0f, 0.0f), textureColor * ndl);
	output = temp;

	ndl = dot( normal.xyz, (dirLights[1].Direction) );
	temp = textureColor * ndl;
	temp = max(float4(0.0f, 0.0f, 0.0f, 0.0f), textureColor * ndl);
	output += temp;

	ndl = dot( normal.xyz, (dirLights[2].Direction) );
	temp = textureColor * ndl;
	temp = max(float4(0.0f, 0.0f, 0.0f, 0.0f), textureColor * ndl);
	output += temp;

	output.w = 1.0f;

    return output;
}