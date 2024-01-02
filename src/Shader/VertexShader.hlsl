#include "LightHelper.hlsl"

cbuffer cbPerObject
{
    matrix g_world;
    matrix g_wvp;
    matrix g_worldInvTranspose;
	Material g_material;
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
    vout.pos = mul(vout.pos, g_wvp);
    vout.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return vout;
}