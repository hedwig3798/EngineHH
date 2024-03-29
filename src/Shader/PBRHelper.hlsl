// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (3.141592 * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor. 최소값 F0 , 최대값은 1.0,1.0,1.0
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


/// 용준 버전
float3 Fresnel(float HdotV, float3 F0)
{   
    float viewSeta = pow((1 - HdotV), 5);
    float3 reflect = (1 - F0) * viewSeta;
    
    return F0 + reflect;
}

float NDF(float vNdotH, float roughness)
{
    float result = pow(roughness + 0.01f, 2);
    float rough = pow((((result - 1) * pow(vNdotH,2)) + 1), 2) * 3.141592;
    
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