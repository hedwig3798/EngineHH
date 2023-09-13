#pragma once

#include "headers.h"
/// <summary>
/// 벡터 클래스
/// 최초 작성일 : 2023/07/20
/// 최종 작성일 : 2023/09/12
/// 작성자 : 김형환
/// 
/// 2D 벡터 연산을 위한 클래스
/// </summary>

class H4x4Matrix;
class Vector3;
class Vector4;

class Vector2
{
public:
	// 생성자
	Vector2(int _a, int _b);
	Vector2(float _a = 0.f, float _b = 0.f);
	Vector2(const Vector3& _vec3);
	Vector2(const Vector4& _vec4);

	~Vector2();

	// 값
	float x;
	float y;

public:
	// 연산자 오버로딩
	Vector2 operator+(const Vector2& _other) const;

	Vector2 operator-(const Vector2& _other) const;

	float operator*(const Vector2& _other) const;
	Vector2 operator*(const float& _other) const;

	Vector2 operator/(const float& _scale) const;
	Vector2 operator/(const int& _scale) const;

	void operator+=(const Vector2& _other);

	void operator-=(const Vector2& _other);

	void operator*=(const float& _other);

	Vector2 operator-();
	Vector2 operator-() const;

	bool operator==(const Vector2& _other) const;

	// 루트 제곱근 (길이)
	float Length() const;
	float Dot(const Vector2& _other) const;
	void Normalize();
	Vector2 Cross(const Vector2& _other);
};

