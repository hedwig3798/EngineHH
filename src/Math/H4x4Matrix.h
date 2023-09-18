#pragma once
#include "headers.h"
#include <d3d11.h>
#include <DirectXMath.h>

/// <summary>
/// ��� Ŭ����
/// ���� �ۼ��� : 2023/09/08
/// ���� �ۼ��� : 2023/09/11
/// �ۼ��� : ����ȯ
/// 
/// 4 x 4 �������
/// ��� ���꿡 ���� �Լ� ����
/// </summary>

class Vector2;

class H4x4Matrix
{
public:
	float matrix[4][4];
	DirectX::XMMATRIX a = DirectX::XMMATRIX();
public:
	H4x4Matrix();
	~H4x4Matrix();
	H4x4Matrix(const H4x4Matrix& _other);
	// �׵���� �����
	void Setidentity();

	// �̵�
	void Translation(const Vector2& _distance);
	// ��ġ����
	void SetPosition(const Vector2& _distance);
	// ȸ�� ( �߽��� ���� )
	void SetRotation(float _angle, const Vector2& _center, int _axis);
	// ȸ�� ( ���� ���� )
	void SetRotation(float _angle, int _axis);
	// ũ�� ( �߽��� ���� )
	void SetScale(const Vector2& _scale, const Vector2& _center);
	// ũ�� ( ���� ���� )
	void SetScale(const Vector2& _scale);
	// ��½� ���ϱ�
	float GetDetermination();
	// �����ȭ
	void Inverse();
	// ������� ���� ����
	H4x4Matrix GetInverse() const;

	// ��� �����
	float* operator[](int _index);
	const float* operator[](int _index) const;

	H4x4Matrix operator+(const H4x4Matrix& _other);
	H4x4Matrix operator+(const int _other);

	H4x4Matrix operator*(const H4x4Matrix& _other);
	H4x4Matrix operator*(const float _other);
	Vector2 operator*(const Vector2& _matrix) const;

	void operator*=(const H4x4Matrix& _other);
};

