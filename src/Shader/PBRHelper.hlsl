float saturate(float x)
{
	return max(x, 0.0);
}

float3 saturate(float3 x)
{
	return max(x, float3(0.0f, 0.0f, 0.0f));
}

float getLen(float3 _v)
{
	return sqrt(_v.x * _v.x + _v.y * _v.y + _v.z * _v.z); 
}

float3 GetHVector(float3 l, float3 v)
{
	float3 h = l + v;
	return normalize(h);
}

float DistributionFuntion(float _roughness, float _ndoth)
{
	float r2 = _roughness * _roughness;
	float d = (_ndoth * r2 - _ndoth) * _ndoth + 1.0;
	float result = r2 / (d * d * 3.141592);
	return result;
}

float3 FresnelReflection(float _vdoth, float3 _specColor)
{
	float3 result = _specColor + (1.0f - _specColor) * pow((1.0f - clamp(_vdoth, 0.0f, 1.0f)), 5.0f);
	return result;
}

float3 EnvBRDFApprox(float3 _specColor, float _roughness, float _ndotv)
{
	const float4 c0 = float4(-1, -0.0275, -0.572, 0.022);
	const float4 c1 = float4(1, 0.0425, 1.04, -0.04);
	float4 r = _roughness * c0 + c1;
	float a004 = min(r.x * r.x, exp2(-9.28 * _ndotv)) * r.x + r.y;
	float2 AB = float2(-1.04, 1.04) * a004 + r.zw;
	return _specColor * AB.x + AB.y;
}

float GAF(float _roughness, float _ndotl, float _ndotv)
{
	float r2 = _roughness * _roughness;
	float gv = _ndotl * sqrt(_ndotv * (_ndotv - _ndotv * r2) + r2);
	float gl = _ndotv * sqrt(_ndotl * (_ndotl - _ndotl * r2) + r2);
	return 0.5 / max(gv + gl, 0.00001);
}

