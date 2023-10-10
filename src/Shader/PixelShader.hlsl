
struct VOUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 PS (VOUT _pin) : SV_Target
{
    return _pin.color;
}