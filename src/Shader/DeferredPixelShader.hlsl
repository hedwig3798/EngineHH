#include "../Shader/VertexShader3.hlsl"


Texture2DArray diffuseMaps : register(t0);
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
};

DeferredOutput PS(VertexOut pin)
{
	DeferredOutput output;

	float4 textureColor = diffuseMaps.Sample( samAnisotropic, float3( pin.Tex, pin.texindex ) );
 	textureColor.a = 1.0f;

	output.texDiffuse = textureColor;
	output.texNormal = float4(pin.NormalW, 1.0f);

    return output;
}