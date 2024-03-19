TextureCube FinalTexture : register(t0);

SamplerState f_Sampler : register(s0);

struct SkyboxOutput
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

float4 PS(SkyboxOutput _input) : SV_Target
{
	float4 color = FinalTexture.Sample(f_Sampler, _input.PosL); 
	
	return color; 

}