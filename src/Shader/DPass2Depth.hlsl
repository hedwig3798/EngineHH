#include "../Shader/DPass2VS.hlsl"


Texture2D diffuseMaps[3] : register(t0);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
};

float4 PS(VOUT pin) : SV_Target
{
	float4 textureColor = diffuseMaps[2].Sample(samAnisotropic, pin.Tex);
    return textureColor;
}