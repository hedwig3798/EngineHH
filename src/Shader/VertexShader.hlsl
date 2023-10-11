cbuffer cbPerObject
{
    matrix world;
    matrix wvp;
    matrix worldInversTranspose;
};

struct VIN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VOUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VOUT VS(VIN _vin)
{
    VOUT vout;
    vout.pos = float4(_vin.pos, 1.0f);
    vout.pos = mul(vout.pos, wvp);
    vout.color = _vin.color;
    
    return vout;
}