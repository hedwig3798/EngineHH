
struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
};

Texture2D gDiffuseMap : register(t0);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

float4 PS(VertexOut pin) : SV_Target
{
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);

	// Sample texture.
	//float2 _texCoord = pin.Tex;
	//_texCoord.g = 1 - pin.Tex.g;
	//texColor = gDiffuseMap.Sample(samAnisotropic, _texCoord);

	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    

	// 알파를 강제로 1로, 텍스쳐의 내용만 찍자.
    texColor.a = 1.0f;

    return texColor;
}