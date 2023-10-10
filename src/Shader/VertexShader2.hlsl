cbuffer cbPerObject
{
    matrix w;
    matrix v;
    matrix p;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), w);
		
    matrix wvp = w;
    wvp = mul(wvp, v);
    wvp = mul(wvp, p);

	vout.PosW = mul(float4(vin.PosL, 1.0f), w).xyz;
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), wvp);
	
	// �ؽ����� ��ǥ (texcoord�� �״�� �ȼ����̴��� �Ѱ� �ش�)
	vout.Tex = vin.Tex;

	return vout;
}