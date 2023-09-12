#include "H4x4Matrix.h"
#include "Vector2.h"
#include <iostream>

/// <summary>
/// 최초 생성시 4x4 정방항등행렬
/// </summary>
/// <param name="_size"></param>
H4x4Matrix::H4x4Matrix()
	: matrix(4, std::vector<float>(4))
{
	Setidentity();
}


H4x4Matrix::~H4x4Matrix()
{
}

/// <summary>
/// 행렬 데이터를 항등행렬로 변경
/// </summary>
void H4x4Matrix::Setidentity()
{
	for (size_t i = 0; i < this->matrix.size(); i++)
	{
		for (size_t j = 0; j < this->matrix[i].size(); j++)
		{
			matrix[i][j] = 0;
			if (i == j)
			{
				matrix[i][j] = 1;
			}
		}
	}
}

/// <summary>
/// 움직임 함수
/// </summary>
/// <param name="_distance">해당 """거리"""만큼 이동</param>
void H4x4Matrix::Translation(const Vector2& _distance)
{
	// 항등행렬을 가져와 이동 행렬 구현
	H4x4Matrix temp;

	temp[3][0] = _distance.x;
	temp[3][1] = _distance.y;
	temp[3][2] = 0.f;

	// 이동
	*this *= temp;
}

void H4x4Matrix::SetPosition(const Vector2& _distance)
{
	// 항등행렬을 가져와 이동 행렬 구현
	H4x4Matrix temp;

	temp[3][0] = _distance.x;
	temp[3][1] = _distance.y;
	temp[3][2] = 0.f;

	// 이동
	*this = temp;
}

/// <summary>
/// 회전 함수
/// </summary>
/// <param name="_angle">각도 (일반각)</param>
/// <param name="_axis">축 (x : 1 / y : 2 / z : 3) 기본 z축 회전</param>
void H4x4Matrix::SetRotation(float _angle, int _axis)
{
	// 항등행렬 구하기
	H4x4Matrix temp;

	// 최전 축에 따라 갑 지정
	switch (_axis)
	{
		// x
		case 1:
		{
			temp[1][1] = cos(_angle);
			temp[1][2] = sin(_angle);
			temp[2][1] = -sin(_angle);
			temp[2][2] = cos(_angle);
		}
		break;

		// y
		case 2:
		{
			temp[0][0] = cos(_angle);
			temp[0][2] = sin(_angle);
			temp[2][0] = -sin(_angle);
			temp[2][2] = cos(_angle);
		}
		break;
		// z
		case 3:
		{
			temp[0][0] = cos(_angle);
			temp[0][1] = sin(_angle);
			temp[1][0] = -sin(_angle);
			temp[1][1] = cos(_angle);
		}
		break;
		default:
			break;
	}

	// 변환 행렬에 곱하기
	*this *= temp;
}

/// <summary>
/// 회전 함수
/// </summary>
/// <param name="_angle">각도 (일반각)</param>
/// <param name="_center">중심 축</param>
/// <param name="_axis">회전 축</param>
void H4x4Matrix::SetRotation(float _angle, const Vector2& _center, int _axis)
{
	// 축을 기준으로 원점으로 옮긴 후 회전 후 다시 원래 위치로 옮기기 (TRT)
	SetPosition(_center);
	SetRotation(_angle, _axis);
	SetPosition(-_center);
}

/// <summary>
/// 크기 조절 함수
/// </summary>
/// <param name="_scale">"""비율"""</param>
void H4x4Matrix::SetScale(const Vector2& _scale)
{
	// 항등행렬 구하기
	H4x4Matrix temp;

	// 스케일 값
	temp[0][0] *= _scale.x;
	temp[1][1] *= _scale.y;

	*this *= temp;
	return;
}

/// <summary>
/// 물체 기준으로 크기 조절 함수
/// </summary>
/// <param name="_scale">비율</param>
/// <param name="_center">축</param>
void H4x4Matrix::SetScale(const Vector2& _scale, const Vector2& _center)
{
	SetPosition(_center);
	SetScale(_scale);
	SetPosition(-_center);
}

/// <summary>
/// 이 행렬의 행렬식 구하기
/// </summary>
/// <returns>행렬식</returns>
float H4x4Matrix::GetDetermination()
{
	// 최초 이전 단계 지지 요소
	float p1 = this->matrix[0][0];
	float p2 = 1;

	H4x4Matrix temp;
	H4x4Matrix original;
	temp.matrix = this->matrix;
	original.matrix = this->matrix;

	// 행렬의 모든 요소를 3번 지나야된다.
	// k = 기준점, i = 열, j = 열
	for (int k = 0; k < 4; k++)
	{
		p2 = p1;
		p1 = temp.matrix[k][k];
		// 행렬 순회
		for (int i = 0; i < 4; i++)
		{
			if (i == k)
			{
				continue;
			}
			original.matrix = temp.matrix;
			for (int j = 0; j < 4; j++)
			{
				temp[i][j] = 
					((original.matrix[k][k] * original.matrix[i][j]) -
					(original.matrix[k][j] * original.matrix[i][k])) / p2;
			}
			
		}
	}
	return temp[3][3];
}

/// <summary>
/// 현재 행렬을 역행렬화
/// </summary>
void H4x4Matrix::SetInverse()
{

}

/// <summary>
/// 현재 행렬의 역행렬을 반환
/// </summary>
/// <returns>역행렬</returns>
H4x4Matrix H4x4Matrix::GetInverse() const
{
	H4x4Matrix temp;
	return temp;
}

#pragma region operator

std::vector<float>& H4x4Matrix::operator[](int _index)
{
	return this->matrix[_index];
}

const std::vector<float>& H4x4Matrix::operator[](int _index) const
{
	return this->matrix[_index];
}

H4x4Matrix H4x4Matrix::operator+(const H4x4Matrix& _other)
{
	assert(this->matrix.size() == _other.matrix.size() && "diffrent matrix size");

	H4x4Matrix temp;

	for (int i = 0; i < this->matrix.size(); i++)
	{
		for (int j = 0; j < this->matrix.size(); j++)
		{
			temp[i][j] = (*this)[i][j] + _other[i][j];
		}
	}

	return temp;
}

H4x4Matrix H4x4Matrix::operator+(const int _other)
{
	H4x4Matrix temp;
	for (int i = 0; i < this->matrix.size(); i++)
	{
		for (int j = 0; j < this->matrix.size(); j++)
		{
			temp[i][j] = (*this)[i][j] + _other;
		}
	}
	return temp;
}

H4x4Matrix H4x4Matrix::operator*(const H4x4Matrix& _other)
{
	assert(this->matrix.size() == _other.matrix.size() && "diffrent matrix size to muliply");

	H4x4Matrix result = _other;
	for (int i = 0; i < this->matrix.size(); i++)
	{
		for (int j = 0; j < this->matrix.size(); j++)
		{
			float acc = 0.0f;
			for (int k = 0; k < this->matrix.size(); k++)
			{
				acc += _other[i][k] * (*this)[k][j];
			}
			result[i][j] = acc;
		}
	}
	return result;
}

H4x4Matrix H4x4Matrix::operator*(const int _other)
{
	H4x4Matrix temp;
	for (int i = 0; i < this->matrix.size(); i++)
	{
		for (int j = 0; j < this->matrix.size(); j++)
		{
			temp[i][j] = (*this)[i][j] * _other;
		}
	}
	return temp;
}

void H4x4Matrix::operator*=(const H4x4Matrix& _other)
{
	(*this) = (*this) * _other;
}

// TODO: 최적화
Vector2 H4x4Matrix::operator*(const Vector2& _vector) const
{
	H4x4Matrix temp;
	temp[3][0] = _vector.x;
	temp[3][1] = _vector.y;

	temp *= (*this);

	return Vector2{ temp[3][0], temp[3][1] };
}

#pragma endregion
