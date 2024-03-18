#include "../Shader/DPass2VS.hlsl"
#include "LightHelper.hlsl"

Texture2D gBuffer[6] : register(t0);

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
	float4 albedo = gBuffer[0].Sample(samAnisotropic, pin.Tex);
	float4 normal = gBuffer[1].Sample(samAnisotropic, pin.Tex);
	normal = (normal * 2.0f) - 1.0f;
	float4 depth = gBuffer[2].Sample(samAnisotropic, pin.Tex);
	float4 worldPos = gBuffer[3].Sample(samAnisotropic, pin.Tex);
	float4 metailic = gBuffer[4].Sample(samAnisotropic, pin.Tex);
	float4 roughness = gBuffer[5].Sample(samAnisotropic, pin.Tex);

	float3 toEye = g_eyePosW-worldPos;
	toEye = normalize(toEye);
	// 그 길이

	DirectionalLight dirLights[3];
	
	Material mat;
	mat.Ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	mat.Diffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
	mat.Specular =  float4(0.8f, 0.8f, 0.8f, 16.0f);

	dirLights[0].Ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLights[0].Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLights[0].Specular = float4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[0].Direction = float3(0.0f, -1.0f, -0.0f);

    float4 litColor = albedo;


	// Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 일단 빞은 하나로 간주한다.
    // Sum the light contribution from each light source.  
    [unroll]
    for (int i = 0; i < 1; ++i)
    {
        // 디퍼드에서는 이미 노말맵 등이 반영되어 GBuffer에 들어있다.
        
        // The light vector aims opposite the direction the light rays travel.
        float3 lightVec = -dirLights[i].Direction;
        lightVec = normalize(lightVec);

	    // Add ambient term.
        ambient += mat.Ambient * dirLights[i].Ambient;

	    // Add diffuse and specular term, provided the surface is in 
	    // the line of site of the light.
	
        float diffuseFactor = dot(lightVec, normal);

	    // Flatten to avoid dynamic branching.
    	[flatten]
        if (diffuseFactor > 0.0f)
        {
            float3 v = reflect(-lightVec, normal);
            float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
            diffuse += diffuseFactor * mat.Diffuse * dirLights[i].Diffuse;
            spec += specFactor * mat.Specular * dirLights[i].Specular;
        }
    }
	litColor = albedo * (ambient + diffuse) + spec;

 	//float4 output;
 
 	//float4 temp;	
 	//float ndl = dot( normal.xyz, (normalize(dirLights[0].Direction)) );
 	//temp = max(float4(0.0f, 0.0f, 0.0f, 0.0f), albedo * ndl);
 
 	//output = temp;
 
 	//output.w = 1.0f;

    return litColor;
}