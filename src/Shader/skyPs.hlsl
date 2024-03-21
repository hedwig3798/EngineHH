TextureCube FinalTexture : register(t0);

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct SkyboxOutput
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

float4 PS(SkyboxOutput _input) : SV_Target
{
	float4 color = FinalTexture.Sample(samTriLinearSam, _input.PosL); 
	
	return color; 

}