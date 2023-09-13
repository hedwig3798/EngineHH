#pragma once

#include "headers.h"
/// <summary>
/// ���� Ŭ����
/// ���� �ۼ��� : 2023/07/20
/// ���� �ۼ��� : 2023/09/12
/// �ۼ��� : ����ȯ
/// 
/// 2D ���� ������ ���� Ŭ����
/// </summary>

class H4x4Matrix;
class Vector3;
class Vector4;

class Vector2
{
public:
	// ������
	Vector2(int _a, int _b);
	Vector2(float _a = 0.f, float _b = 0.f);
	Vector2(const Vector3& _vec3);
	Vector2(const Vector4& _vec4);

	~Vector2();

	// ��
	float x;
	float y;

public:
	// ������ �����ε�
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

	// ��Ʈ ������ (����)
	float Length() const;
	float Dot(const Vector2& _other) const;
	void Normalize();
	Vector2 Cross(const Vector2& _other);
};

