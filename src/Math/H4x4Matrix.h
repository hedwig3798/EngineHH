#pragma once
#include "headers.h"
#include <d3d11.h>

/// <summary>
/// 행렬 클래스
/// 최초 작성일 : 2023/09/08
/// 최초 작성일 : 2023/09/11
/// 작성자 : 김형환
/// 
/// 4 x 4 정방행렬
/// 행렬 연산에 대한 함수 제공
/// </summary>

class Vector2;

class H4x4Matrix
{
public:
	std::vector<std::vector<float>> matrix;

public:
	H4x4Matrix();
	~H4x4Matrix();

	// 항등행렬 만들기
	void Setidentity();

	// 이동
	void Translation(const Vector2& _distance);
	// 위치설정
	void SetPosition(const Vector2& _distance);
	// 회전 ( 중심점 기준 )
	void SetRotation(float _angle, const Vector2& _center, int _axis);
	// 회전 ( 영점 기준 )
	void SetRotation(float _angle, int _axis);
	// 크기 ( 중심점 기준 )
	void SetScale(const Vector2& _scale, const Vector2& _center);
	// 크기 ( 원점 기준 )
	void SetScale(const Vector2& _scale);
	// 행력식 구하기
	float GetDetermination();
	// 역행렬화
	void SetInverse();
	// 역행렬을 만들어서 제공
	H4x4Matrix GetInverse() const;

	// 행렬 연산들
	std::vector<float>& operator[](int _index);
	const std::vector<float>& operator[](int _index) const;

	H4x4Matrix operator+(const H4x4Matrix& _other);
	H4x4Matrix operator+(const int _other);

	H4x4Matrix operator*(const H4x4Matrix& _other);
	H4x4Matrix operator*(const int _other);
	Vector2 operator*(const Vector2& _matrix) const;

	void operator*=(const H4x4Matrix& _other);
};

