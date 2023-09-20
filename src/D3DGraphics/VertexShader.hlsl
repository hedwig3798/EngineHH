cbuffer cbPerObject
{
    matrix w;
    matrix v;
    matrix p;
};


struct Vertexln
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(Vertexln vin)
{
    VertexOut vout;
    float4 vinpos = {vin.PosL.x, vin.PosL.y, vin.PosL.z, 1.0f};
    vout.PosH.w = 1.0f;
    vout.PosH = mul(vinpos, w);
    vout.PosH = mul(vout.PosH, v);
    vout.PosH = mul(vout.PosH, p);
    

    vout.Color = vin.Color;
    return vout;
}