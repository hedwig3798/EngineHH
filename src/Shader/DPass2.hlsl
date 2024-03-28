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
	float4 albedo = gBuffer[0].Sample(samAnisotropic, pin.Tex);
	float4 normal0 = gBuffer[1].Sample(samAnisotropic, pin.Tex);
	float3 normal = normalize(float3(normal0.xyz * 2.0f) - 1.0f);
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
	dirLights[0].Direction = float3(0.0f, -1.0f, 0.5f);

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

	float3 Lo = normalize(toEye);
	float cosLo = max(0.0f, dot(normal, Lo));

	float3 Lr = 2.0 * cosLo * normal - Lo;

	float3 F0 = lerp(Fdielectric, albedo, metailic);	

    [unroll]
    for (int i = 0; i < 1; ++i)
    {
        // 디퍼드에서는 이미 노말맵 등이 반영되어 GBuffer에 들어있다.
        
        // The light vector aims opposite the direction the light rays travel.
        float3 Li = -dirLights[i].Direction;
        Li = normalize(Li);

		// Half-vector between Li and Lo.
		float3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(normal, Li));
		float cosLh = max(0.0, dot(normal, Lh));

		// Calculate Fresnel term for direct lighting. 
		float3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));  //최소값 F0 , 최대값은 1.0,1.0,1.0
		// Calculate normal distribution for specular BRDF.
        float D = ndfGGX(cosLh, max(0.01, roughness));		// 러프니스 0 이되면 값이0이 되므로 0이면 최소값사용
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(cosLi, cosLo, roughness);
	
		float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metailic);
		float3 diffuseBRDF = kd * albedo / PI;

		// Cook-Torrance specular microfacet BRDF.
		float3 specularBRDF = (F * (D * 3) * G) / max(Epsilon, 4.0 * cosLi * cosLo);
		
		// Total contribution for this light.
        color += (diffuseBRDF + specularBRDF) * 5 * cosLi;

		//color = pow(color * 0.4f, 1.0f / 2.2f);
		test = float4(color, 1.0f);
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

    return float4(test.xyz * albedo.xyz, albedo.w);
}