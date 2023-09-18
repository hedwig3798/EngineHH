struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 SimplePixelShader(PixelShaderInput input) : SV_TARGET
{
    return input.color;
}