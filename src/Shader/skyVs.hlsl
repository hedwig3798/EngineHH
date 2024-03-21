
cbuffer cbPerFrame : register(b0)
{
	Matrix w; // ī�޶��� ȸ���̶� ������ȯ�� �ݿ����ش�.
	Matrix vp; // ī�޶��� ȸ���̶� ������ȯ�� �ݿ����ش�.
	Matrix p; // ī�޶��� ȸ���̶� ������ȯ�� �ݿ����ش�.
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

	vout.PosL = _input.PosL; 

	return vout;
}