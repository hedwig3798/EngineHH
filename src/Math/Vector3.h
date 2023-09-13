#pragma once
#include "headers.h"
/// <summary>
/// ���� Ŭ����
/// ���� �ۼ��� : 2023/09/12
/// ���� �ۼ��� : 2023/09/12
/// �ۼ��� : ����ȯ
/// 
/// 3D ���� ������ ���� Ŭ����
/// 3���� ����
/// </summary>

class Vector2;
class Vector4;

class Vector3
{
public:
	// ������
	Vector3(float _a = 0.f, float _b = 0.f, float _c = 0.f);
	Vector3(Vector4 _vec3);
	Vector3(Vector2 _vec2);
	~Vector3();

	// ��
	float x;
	float y;
	float z;

public:
	// ������ �����ε�
	Vector3 operator+(const Vector3& _other) const;

	Vector3 operator-(const Vector3& _other) const;

	float operator*(const Vector3& _other) const;
	Vector3 operator*(const float& _other) const;

	Vector3 operator/(const float& _other) const;

	void operator+=(const Vector3& _other);

	void operator-=(const Vector3& _other);

	void operator*=(const float& _other);

	Vector3 operator-();
	Vector3 operator-() const;

	bool operator==(const Vector3& _other) const;

	// ��Ʈ ������ (����)
	float Length() const;
	float Dot(const Vector3& _other) const;
	void Normalize();
	Vector3 Cross(const Vector3& _other);
};

