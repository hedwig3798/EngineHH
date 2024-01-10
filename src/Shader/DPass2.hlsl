#include "../Shader/DPass2VS.hlsl"


Texture2DArray diffuseMaps : register(t0);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

float4 PS(VOUT pin) : SV_Target
{
	float4 textureColor = diffuseMaps.Sample( samAnisotropic, float3( pin.Tex, 0.0f ) );
	float4 normal = diffuseMaps.Sample( samAnisotropic, float3( pin.Tex, 1.0f ) );

    return textureColor;
    // return normal;
}