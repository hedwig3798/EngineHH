cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    
    float4 WorldDirectionLight;
    float4 CameraPosition;
	
    float4 AmbientColor;
    float4 DiffuseColor;
    float4 SpecularColor;
	
    float4 MaterialSpecularPower;
    
    int isNormal;
    int isSpecular;
    int isGamma;
    float Alpha;
    
    matrix local;
    float3 CameraForward;
    float LightPower;
    
    float Gamma;
    float Gamma1; // padding
    float Metallic;
    float Roughness;
    
}

cbuffer BoneBuffer : register(b1)
{
    matrix BoneTransform[100];
    float4 c1[2];
    float metalness;
    float roughness;
    float lightPower;
    float fadding;
}

cbuffer ShadowBuffer : register(b2)
{
    matrix ShadowView;
    matrix ShadowProjection;
}


//cbuffer PBRBuffer : register(b2)
//{
//    float metalness;
//    float roughness;
//    float2 fadding;
//}

// NPBR
Texture2D txNone : register(t0);
Texture2D txDiffuseMap : register(t1);
Texture2D txSpecularMap : register(t2);
Texture2D txAmbientMap : register(t3);
Texture2D txEmissiveMap : register(t4);
Texture2D txHeightMap : register(t5);
Texture2D txNormalMap : register(t6);
Texture2D txShininessMap : register(t7);
Texture2D txOpacityMap : register(t8);
Texture2D txDisplacement : register(t9);
Texture2D txLightMap : register(t10);
Texture2D txReflectionMap : register(t11);
// PBR
Texture2D txColorMap : register(t12);
Texture2D txRoughness : register(t13);
Texture2D tx1 : register(t14);
Texture2D txMetalness : register(t15);
Texture2D tx2 : register(t16);

Texture2D txShadowMap : register(t20);



TextureCube txIBL_EnvMap : register(t30);
TextureCube txIBL_DiffuseMap : register(t31);
TextureCube txIBL_SpecularMap : register(t32);
Texture2D txIBL_SpecularBRDF_LUT : register(t33);



SamplerState samLinear : register(s0);
SamplerState clampLinear : register(s1);


#define PI_ 3.141592

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 UVW : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    uint MaterialIndex : MATERIALINDEX;
    float3 Weights : WEIGHTS;
    uint4 BoneIndices : BONEINDICES;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT;
    float3 BiTangent : BITANGENT;
    float3 UVW : TEXCOORD;
    uint MaterialIndex : MATERIALINDEX;
    float4 PosShadow : POSITION1;
};

struct SkyBox_VS_Input
{
    float4 Pos : POSITION;
    float3 UVW : TEXCOORD;
};

struct SkyBox_PS_Input
{
    float4 Pos : SV_POSITION;
    float3 UVW : TEXCOORD;
};