#include "DPass2VS.hlsl"
#include "LightHelper.hlsl"
#include "PBRHelper.hlsl"

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
	float metailic = gBuffer[4].Sample(samAnisotropic, pin.Tex).x;
	float roughness = gBuffer[5].Sample(samAnisotropic, pin.Tex).x;

	float3 toEye = g_eyePosW-worldPos;
	toEye = normalize(toEye);
	// 그 길이

	DirectionalLight dirLights[3];
	
	Material mat;
	mat.Ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	mat.Diffuse = float4(0.3f, 0.3f, 0.3f, 1.0f);
	mat.Specular =  float4(0.8f, 0.8f, 0.8f, 16.0f);

	dirLights[0].Ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLights[0].Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLights[0].Specular = float4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[0].Direction = float3(0.0f, -1.0f, 0.0f);

    float4 litColor = albedo;
	float3 lightColor = float3(2.0f,2.0f,2.0f);

	// Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 spec = float3(0.0f, 0.0f, 0.0f);

	float4 test = {0.0f, 0.0f, 0.0f, 1.0f};
	float3 baseMetal = {0.0003,0.0003,0.0003};
	// 일단 빞은 하나로 간주한다.
    // Sum the light contribution from each light source.  

	float3 color = float3(0.0f, 0.0f, 0.0f);

    [unroll]
    for (int i = 0; i < 1; ++i)
    {
        // 디퍼드에서는 이미 노말맵 등이 반영되어 GBuffer에 들어있다.
        
        // The light vector aims opposite the direction the light rays travel.
        float3 lightVec = -dirLights[i].Direction;
        lightVec = normalize(lightVec);

		float3 halfVec = normalize(toEye + lightVec);
		float vdoth = saturate(dot(toEye, halfVec));
		float ndoth = (dot(normal, halfVec));
		float ndotv = saturate(dot(normal, toEye));
		float ndotl = saturate(dot(normal, lightVec));
	

		float3 h = GetHVector(lightVec, toEye);
		float D = DistributionFuntion(roughness, ndoth);
		float G = GAF(roughness, ndotl, ndotv);
		float3 F = FresnelReflection(vdoth, lerp(baseMetal, albedo.xzy, metailic));
		float3 approx = EnvBRDFApprox(lerp(baseMetal, albedo, metailic.x), roughness, ndotv);
		//spec += approx;
		spec +=  (D * F * G) / (4 * ndotl * ndotv);
		//spec *= pow(dot(normal, toEye), roughness);
		float3 diffuseColor = lerp(float3(0.0f, 0.0f, 0.0f), albedo.xyz, 1 - metailic);
		diffuse += diffuseColor;	
		// diffuse += diffuseColor * dot(normal, lightVec);	

		color = (spec + diffuse) * ndotl;
		//color = pow(color * 0.4f, 1.0f / 2.2f);
		test = float4(F, 1.0f);
// 	    // Add ambient term.
//         ambient += mat.Ambient * dirLights[i].Ambient;
// 
// 	    // Add diffuse and specular term, provided the surface is in 
// 	    // the line of site of the light.
// 	
//         float diffuseFactor = dot(lightVec, normal);
// 
// 	    // Flatten to avoid dynamic branching.
//     	[flatten]
//         if (diffuseFactor > 0.0f)
//         {
//             float3 v = reflect(-lightVec, normal);
//             float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
// 					
//             diffuse += diffuseFactor * mat.Diffuse * dirLights[i].Diffuse;
//             spec += specFactor * mat.Specular * dirLights[i].Specular;
//         }
    }



// 	litColor = albedo * (ambient + diffuse + spec);
// 
//  	//float4 output;
//  
//  	//float4 temp;	
//  	//float ndl = dot( normal.xyz, (normalize(dirLights[0].Direction)) );
//  	//temp = max(float4(0.0f, 0.0f, 0.0f, 0.0f), albedo * ndl);
//  
//  	//output = temp;
//  
//  	//output.w = 1.0f;

    return float4(test.xyz, albedo.w);
}