#pragma once
#include "headers.h"

/// <summary>
/// ���� Ŭ����
/// ���� �ۼ��� : 2023/09/12
/// ���� �ۼ��� : 2023/09/12
/// �ۼ��� : ����ȯ
/// 
/// 3D ���� ������ ���� Ŭ����
/// 4���� ����
/// </summary>

class Vector2;
class Vector3;

class Vector4
{
public:
	// ������
	Vector4(float _a = 0.f, float _b = 0.f, float _c = 0.f, float _d = 0.f);
	Vector4(Vector3 _vec3);
	Vector4(Vector2 _vec2);
	~Vector4();

	// ��
	float x;
	float y;
	float z;
	float w;

public:
	// ������ �����ε�
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

	// ��Ʈ ������ (����)
	float Length() const;
	float Dot(const Vector4& _other) const;
	void Normalize();
	Vector3 Cross3(const Vector4& _other);
};

