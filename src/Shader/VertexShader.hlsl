cbuffer cbPerObject
{
    matrix w;
    matrix v;
    matrix p;
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
    vout.pos = mul(vout.pos, w);
    vout.pos = mul(vout.pos, v);
    vout.pos = mul(vout.pos, p);
    vout.color = _vin.color;
    
    return vout;
}