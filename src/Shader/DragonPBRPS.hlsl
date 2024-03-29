#include <Header.hlsli>


float3 Fresnel(float HdotV, float3 F0)
{   
    float viewSeta = pow((1 - HdotV), 5);
    float3 reflect = (1 - F0) * viewSeta;
    
    return F0 + reflect;
}

float NDF(float vNdotH, float roughness)
{
    float result = pow(roughness + 0.01f, 2);
    float rough = pow((((result - 1) * pow(vNdotH,2)) + 1), 2) * PI_;
    
    return result / rough;
}

float GAF(float NdotL, float NdotV, float roughness)
{
    float kIBL = ((pow(roughness + 0.01f, 2) / 2) + (pow(roughness + 1.01f, 2) / 8)) / 2;
    float GV = NdotV / (((NdotV) * (1 - kIBL)) + kIBL);
    float GL = NdotL / (((NdotL) * (1 - kIBL)) + kIBL);
    //return GV * GL;
    return min(1, min(abs(GV), abs(GL)));
}

float GAF2(float NdotL, float NdotV, float NdotH,float VdotH)
{
    return min(1, min(abs(2 * NdotH * NdotV / VdotH), abs(2 * NdotH * NdotL / VdotH)));
}

float4 main(PS_INPUT input) : SV_Target
{
    float4 FinalColor = { 1,1,1,1};
    //FinalColor = txEnvMap.Sample(samLinear, CameraForward + input.WorldPos);
    float3 PBRs = 0,PBRd = 0;
    
    // half, view, normal, light, basecolor;
    float4 baseColor = txDiffuseMap.Sample(samLinear, input.UVW.xy) * 1;

    if (length(baseColor) <0.001f)
    {
        baseColor = (1, 1, 1,1);

    }
        baseColor.rgb = baseColor.rgb * baseColor.rgb;
    
    float4 opacity = txOpacityMap.Sample(samLinear, input.UVW.xy);
    clip((1 - (opacity.a - opacity.r)) -0.355f);
    float3 view = CameraPosition - input.WorldPos;
    view = normalize(view);
    float3 halfVector = normalize((-WorldDirectionLight.xyz) + view.xyz);
    
    float3 normal = input.Normal;
    if (isNormal & 1)
    {
        float3 vNormalMap = txNormalMap.Sample(samLinear, input.UVW.xy);
        vNormalMap = vNormalMap * 2.0f - 1.0f;
        float3x3 vTangentSpace = float3x3(input.Tangent, input.BiTangent, input.Normal);
        normal = mul(vNormalMap, vTangentSpace);
    }
    normal = normalize(normal);
    float NdotL = max(0, dot(normal, -WorldDirectionLight.xyz));
    
    float NdotH = (dot(normal, halfVector));
    float NdotV = (dot(normal, view));
    
    //if (NdotV < 0.2f)
    //{
    //    return float4(0, 0, 0, 1);
    //}
    
    float HdotV = (dot(halfVector, view));

    
    float vMetal = txMetalness.Sample(samLinear, input.UVW.xy).x;
    vMetal += (1.0f - vMetal) * Metallic;

    float vRough = txShininessMap.Sample(samLinear, input.UVW.xy).x;
    vRough += (1.0f - vRough) * Roughness;
    
    
    float3 F0 = ((0.04f * 1) * (1 - vMetal)) + (baseColor.rgb * vMetal);
    
    float3 F = Fresnel(HdotV, F0);
    float D = NDF(NdotH, vRough * vRough);
    float G = GAF2(NdotL,NdotV,NdotH,HdotV);
    
    float3 VrefN = reflect(-view, normal).xyz;
    float3 envColor = txIBL_EnvMap.Sample(samLinear, VrefN).rgb * vRough;
    
    
    PBRs = (F * D * G) / (4 * NdotL * NdotV);
    PBRs = lerp(PBRs, envColor, vMetal);
    
    float3 kd = lerp(1.0 - F, 0.04, vMetal);
    
    PBRd = kd * baseColor.rgb;

    float3 PBR = ((PBRs + PBRd) * NdotL * 1) * (SpecularColor.rgb * LightPower);
    //return FinalColor;
    //FinalColor.rgb = sqrt(FinalColor.rgb);
    
    //return FinalColor;
    
    uint specularTextureLevels;
    uint width, height;
    txIBL_SpecularMap.GetDimensions(0, width, height, specularTextureLevels);
    float3 specularIrradiance = txIBL_SpecularMap.SampleLevel(samLinear, VrefN, vRough * specularTextureLevels).rgb;
    float2 specularBRDF = txIBL_SpecularBRDF_LUT.Sample(clampLinear, float2(NdotV, vRough)).rg;
    
    
    //float3 Fs = (((F0 * specularBRDF.x) + specularBRDF.y) * specularIrradiance);
    //float3 IBLs = lerp(Fs.rgb, ( /**/txIBL_EnvMap.Sample(samLinear, VrefN).rgb/**/ * vRough), vMetal);
    float3 IBLs = (((F0 * specularBRDF.x) + specularBRDF.y) * specularIrradiance);
    IBLs = lerp(IBLs, envColor, vMetal);
    
    
    float3 diffuseIrradiance = txIBL_DiffuseMap.Sample(samLinear, normal).rgb;
    F = Fresnel(NdotV, F0);
    
    kd = lerp(1.0-F,0.0,vMetal);
    
    float3 IBLd = kd * baseColor.rgb * diffuseIrradiance;

    
    float3 IBL = (IBLd + IBLs);
    
    ////Shadow
    float currentShadowDepth = input.PosShadow.z / input.PosShadow.w;
    float2 uv    = input.PosShadow.xy / input.PosShadow.w;
    
    uv.y = -uv.y;
    uv = (uv * 0.5f) + 0.5f;
    
    PBR = saturate(PBR);
    if (uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
    {
        float sampleShadowDepth = txShadowMap.Sample(samLinear, uv).r;
        if (currentShadowDepth > sampleShadowDepth + 0.001)
        {
            PBR = 0.0f;
        }

    }
    IBL = saturate(IBL);
    //FinalColor.rgb = lerp(PBR,IBL,Gamma1);
    FinalColor.rgb = PBR+(IBL*Gamma1);
    
    FinalColor.rgb = sqrt(FinalColor.rgb);
    FinalColor.a = baseColor.a;
    
    FinalColor += txEmissiveMap.Sample(samLinear, input.UVW.xy);
    
    //FinalColor.rgb = round(FinalColor.rgb * 4) / 4;
    return FinalColor;

}

