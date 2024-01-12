#include "../Shader/DPass2VS.hlsl"


Texture2D diffuseMaps[2] : register(t0);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
};

float4 PS(VOUT pin) : SV_Target
{
	float4 normal = diffuseMaps[1].Sample(samAnisotropic, pin.Tex);
    return normal;
}