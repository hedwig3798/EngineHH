
cbuffer cbPerFrame : register(b0)
{
	Matrix w;
	Matrix vp;
	Matrix p; 
};

struct SkyboxInput
{
	float3 PosL : POSITION;
	float2 panding : TEXCOORD;
};

struct SkyboxOutput
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

SkyboxOutput VS(SkyboxInput _input)
{
	SkyboxOutput vout;
    vout.PosH = mul(float4(_input.PosL, 1.0f), (vp)).xyww;
	vout.PosH.z = vout.PosH.w * 0.99999f;
	vout.PosL = _input.PosL; 

	return vout;
}