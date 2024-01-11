#include "../Shader/DPass2VS.hlsl"


Texture2D diffuseMaps[2] : register(t0);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

float4 PS(VOUT pin) : SV_Target
{
	float4 textureColor = diffuseMaps[0].Sample(samAnisotropic, pin.Tex);
	float4 normal = diffuseMaps[1].Sample(samAnisotropic, pin.Tex);

    return textureColor;
    //return normal;
}