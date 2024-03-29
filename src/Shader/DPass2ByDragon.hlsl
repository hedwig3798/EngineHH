#include "DPass2VS.hlsl"
#include "LightHelper.hlsl"
#include "PBRHelper.hlsl"

Texture2D gBuffer[6] : register(t0);

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

static const float3 Fdielectric = 0.04;

cbuffer cbPerFrame : register(b0)
{
	float3 g_eyeDirW;
	float3 g_eyePosW;
	float2 panding;
};

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
};

float4 PS(VOUT pin) : SV_Target
{
	float4 baseColor = gBuffer[0].Sample(samAnisotropic, pin.Tex);
	float4 normal0 = gBuffer[1].Sample(samAnisotropic, pin.Tex);
	float3 normal = normalize(float3(normal0.xyz * 2.0f) - 1.0f);
	float4 depth = gBuffer[2].Sample(samAnisotropic, pin.Tex);
	float4 worldPos = gBuffer[3].Sample(samAnisotropic, pin.Tex);
	float vMetal = gBuffer[4].Sample(samAnisotropic, pin.Tex).x;
	float vRough = gBuffer[5].Sample(samAnisotropic, pin.Tex).x;
		
    DirectionalLight dirLights[3];
	dirLights[0].Ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLights[0].Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLights[0].Specular = float4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[0].Direction = float3(0.0f, -1.0f, 0.5f);	

	float4 FinalColor = { 1,1,1,0};
    //FinalColor = txEnvMap.Sample(samLinear, CameraForward + input.WorldPos);
    float3 PBRs = 0,PBRd = 0;

    if (length(baseColor) <0.001f)
    {
        baseColor = (1, 1, 1,0);

    }
    baseColor.rgb = baseColor.rgb * baseColor.rgb;
   
    float3 view = g_eyePosW - worldPos;
    view = normalize(view);
    float3 halfVector = normalize((-dirLights[0].Direction.xyz) + view.xyz);
   
    normal = normalize(normal);
    float NdotL = max(0, dot(normal, -dirLights[0].Direction.xyz));
    
    float NdotH = (dot(normal, halfVector));
    float NdotV = (dot(normal, view));
    
    //if (NdotV < 0.2f)
    //{
    //    return float4(0, 0, 0, 1);
    //}
    
    float HdotV = (dot(halfVector, view));

//     vMetal += (1.0f - vMetal) * Metallic;
//     vRough += (1.0f - vRough) * Roughness;
    
    
    float3 F0 = ((0.04f * 1) * (1 - vMetal)) + (baseColor.rgb * vMetal);
    
    float3 F = Fresnel(HdotV, F0);
    float D = NDF(NdotH, vRough * vRough);
    float G = GAF2(NdotL,NdotV,NdotH,HdotV);
    
    float3 VrefN = reflect(-view, normal).xyz;
    
    PBRs = (F * D * G) / (4 * NdotL * NdotV);
    
    float3 kd = lerp(1.0 - F, 0.04, vMetal);
    
    PBRd = kd * baseColor.rgb;

    float3 PBR = ((PBRs + PBRd) * NdotL * 1) * (2);
    //return FinalColor;
    // FinalColor.rgb = sqrt(FinalColor.rgb);
    
    return float4(PBR, baseColor.w);
    
//     uint specularTextureLevels;
//     uint width, height;
//     txIBL_SpecularMap.GetDimensions(0, width, height, specularTextureLevels);
//     float3 specularIrradiance = txIBL_SpecularMap.SampleLevel(samLinear, VrefN, vRough * specularTextureLevels).rgb;
//     float2 specularBRDF = txIBL_SpecularBRDF_LUT.Sample(clampLinear, float2(NdotV, vRough)).rg;
//     
//     
//     //float3 Fs = (((F0 * specularBRDF.x) + specularBRDF.y) * specularIrradiance);
//     //float3 IBLs = lerp(Fs.rgb, ( /**/txIBL_EnvMap.Sample(samLinear, VrefN).rgb/**/ * vRough), vMetal);
//     float3 IBLs = (((F0 * specularBRDF.x) + specularBRDF.y) * specularIrradiance);
//     IBLs = lerp(IBLs, envColor, vMetal);
//     
//     
//     float3 diffuseIrradiance = txIBL_DiffuseMap.Sample(samLinear, normal).rgb;
//     F = Fresnel(NdotV, F0);
//     
//     kd = lerp(1.0-F,0.0,vMetal);
//     
//     float3 IBLd = kd * baseColor.rgb * diffuseIrradiance;
// 
//     
//     float3 IBL = (IBLd + IBLs);
//     
//     ////Shadow
//     float currentShadowDepth = input.PosShadow.z / input.PosShadow.w;
//     float2 uv    = input.PosShadow.xy / input.PosShadow.w;
//     
//     uv.y = -uv.y;
//     uv = (uv * 0.5f) + 0.5f;
//     
//     PBR = saturate(PBR);
//     if (uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
//     {
//         float sampleShadowDepth = txShadowMap.Sample(samLinear, uv).r;
//         if (currentShadowDepth > sampleShadowDepth + 0.001)
//         {
//             PBR = 0.0f;
//         }
// 
//     }
//     IBL = saturate(IBL);
//     //FinalColor.rgb = lerp(PBR,IBL,Gamma1);
//     FinalColor.rgb = PBR+(IBL*Gamma1);
//     
//     FinalColor.rgb = sqrt(FinalColor.rgb);
//     FinalColor.a = baseColor.a;
//     
//     FinalColor += txEmissiveMap.Sample(samLinear, input.UVW.xy);
    
    //FinalColor.rgb = round(FinalColor.rgb * 4) / 4;
    //return PBR;

}