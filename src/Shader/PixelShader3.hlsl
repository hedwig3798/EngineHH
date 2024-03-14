#include "../Shader/VertexShader3.hlsl"


Texture2DArray gBuffer : register(t0);
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

float4 PS(VertexOut pin) : SV_Target
{
	// 법선 벡터의 정규화
	pin.NormalW = normalize(pin.NormalW);
	// 물체에서 카메라 까지의 벡터
	float3 toEye = pin.PosW;
	// 그 길이
	float3 distEye = length(toEye);
	// 정규화
	toEye /= distEye;
	
	// 0에서 시작하는 빛
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 test = {1.0f, 1.0f, 1.0f, 1.0f};
	float4 normal = float4(pin.NormalW, 1.0f);
	// Sum the light contribution from each light source.  
//  	[unroll(3)]
//  	for(int i = 0; i < g_lightCount; ++i)
//  	{
// 		float4 A, D, S;
// 		ComputeDirectionalLight(pin.material, g_dirLights[i], pin.NormalW, toEye, A, D, S);
// 
// 		ambient += A;
// 		diffuse += D;
// 		spec    += S;
// 	}
	
	float4 litColor = ambient + diffuse + spec;
	// litColor = ceil(((diffuse + spec) * 10)/2 ) * litColor;
	// litColor.a = pin.material.Diffuse.a;
	
	float4 textureColor = gBuffer.Sample( samAnisotropic, float3( pin.Tex, pin.texindex ) );

 	textureColor.a = 1.0f;

	float4 result = litColor * textureColor;
    return result;
}