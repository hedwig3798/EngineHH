#pragma once
#include "headers.h"

/// <summary>
/// 벡터 클래스
/// 최초 작성일 : 2023/09/12
/// 최종 작성일 : 2023/09/12
/// 작성자 : 김형환
/// 
/// 3D 벡터 연산을 위한 클래스
/// 4차원 벡터
/// </summary>

class Vector2;
class Vector3;

class Vector4
{
public:
	// 생성자
	Vector4(float _a = 0.f, float _b = 0.f, float _c = 0.f, float _d = 0.f);
	Vector4(Vector3 _vec3);
	Vector4(Vector2 _vec2);
	~Vector4();

	// 값
	float x;
	float y;
	float z;
	float w;

public:
	// 연산자 오버로딩
	Vector4 operator+(const Vector4& _other) const;

	Vector4 operator-(const Vector4& _other) const;

	float operator*(const Vector4& _other) const;
	Vector4 operator*(const float& _other) const;

	Vector4 operator/(const float& _other) const;

	void operator+=(const Vector4& _other);

	void operator-=(const Vector4& _other);

	void operator*=(const float& _other);

	Vector4 operator-();
	Vector4 operator-() const;

	bool operator==(const Vector4& _other) const;

	// 루트 제곱근 (길이)
	float Length() const;
	float Dot(const Vector4& _other) const;
	void Normalize();
	Vector3 Cross3(const Vector4& _other);
};

