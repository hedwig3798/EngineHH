
cbuffer cbPerFrame : register(b0)
{
	float4x4 vp; // 카메라의 회전이랑 투영변환만 반영해준다.
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
    vout.PosH = mul(float4(_input.PosL * 2000, 1.0f), vp).xyww;

	vout.PosL = _input.PosL; 

	return vout;
}