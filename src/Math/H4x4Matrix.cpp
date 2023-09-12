#include "H4x4Matrix.h"
#include "Vector2.h"
#include <iostream>

/// <summary>
/// ���� ������ 4x4 �����׵����
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
/// ��� �����͸� �׵���ķ� ����
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
/// ������ �Լ�
/// </summary>
/// <param name="_distance">�ش� """�Ÿ�"""��ŭ �̵�</param>
void H4x4Matrix::Translation(const Vector2& _distance)
{
	// �׵������ ������ �̵� ��� ����
	H4x4Matrix temp;

	temp[3][0] = _distance.x;
	temp[3][1] = _distance.y;
	temp[3][2] = 0.f;

	// �̵�
	*this *= temp;
}

void H4x4Matrix::SetPosition(const Vector2& _distance)
{
	// �׵������ ������ �̵� ��� ����
	H4x4Matrix temp;

	temp[3][0] = _distance.x;
	temp[3][1] = _distance.y;
	temp[3][2] = 0.f;

	// �̵�
	*this = temp;
}

/// <summary>
/// ȸ�� �Լ�
/// </summary>
/// <param name="_angle">���� (�Ϲݰ�)</param>
/// <param name="_axis">�� (x : 1 / y : 2 / z : 3) �⺻ z�� ȸ��</param>
void H4x4Matrix::SetRotation(float _angle, int _axis)
{
	// �׵���� ���ϱ�
	H4x4Matrix temp;

	// ���� �࿡ ���� �� ����
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

	// ��ȯ ��Ŀ� ���ϱ�
	*this *= temp;
}

/// <summary>
/// ȸ�� �Լ�
/// </summary>
/// <param name="_angle">���� (�Ϲݰ�)</param>
/// <param name="_center">�߽� ��</param>
/// <param name="_axis">ȸ�� ��</param>
void H4x4Matrix::SetRotation(float _angle, const Vector2& _center, int _axis)
{
	// ���� �������� �������� �ű� �� ȸ�� �� �ٽ� ���� ��ġ�� �ű�� (TRT)
	SetPosition(_center);
	SetRotation(_angle, _axis);
	SetPosition(-_center);
}

/// <summary>
/// ũ�� ���� �Լ�
/// </summary>
/// <param name="_scale">"""����"""</param>
void H4x4Matrix::SetScale(const Vector2& _scale)
{
	// �׵���� ���ϱ�
	H4x4Matrix temp;

	// ������ ��
	temp[0][0] *= _scale.x;
	temp[1][1] *= _scale.y;

	*this *= temp;
	return;
}

/// <summary>
/// ��ü �������� ũ�� ���� �Լ�
/// </summary>
/// <param name="_scale">����</param>
/// <param name="_center">��</param>
void H4x4Matrix::SetScale(const Vector2& _scale, const Vector2& _center)
{
	SetPosition(_center);
	SetScale(_scale);
	SetPosition(-_center);
}

/// <summary>
/// �� ����� ��Ľ� ���ϱ�
/// </summary>
/// <returns>��Ľ�</returns>
float H4x4Matrix::GetDetermination()
{
	// ���� ���� �ܰ� ���� ���
	float p1 = this->matrix[0][0];
	float p2 = 1;

	H4x4Matrix temp;
	H4x4Matrix original;
	temp.matrix = this->matrix;
	original.matrix = this->matrix;

	// ����� ��� ��Ҹ� 3�� �����ߵȴ�.
	// k = ������, i = ��, j = ��
	for (int k = 0; k < 4; k++)
	{
		p2 = p1;
		p1 = temp.matrix[k][k];
		// ��� ��ȸ
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
/// ���� ����� �����ȭ
/// </summary>
void H4x4Matrix::SetInverse()
{

}

/// <summary>
/// ���� ����� ������� ��ȯ
/// </summary>
/// <returns>�����</returns>
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

// TODO: ����ȭ
Vector2 H4x4Matrix::operator*(const Vector2& _vector) const
{
	H4x4Matrix temp;
	temp[3][0] = _vector.x;
	temp[3][1] = _vector.y;

	temp *= (*this);

	return Vector2{ temp[3][0], temp[3][1] };
}

#pragma endregion
