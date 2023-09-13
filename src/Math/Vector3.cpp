#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"

Vector3::Vector3(float _a /*= 0.f*/, float _b /*= 0.f*/, float _c /*= 0.f*/)
	: x(_a)
	, y(_b)
	, z(_c)
{

}

Vector3::Vector3(Vector4 _vec3)
	: x(_vec3.x)
	, y(_vec3.y)
	, z(_vec3.z)
{

}

Vector3::Vector3(Vector2 _vec2)
	: x(_vec2.x)
	, y(_vec2.y)
	, z(0)
{

}

Vector3::~Vector3()
{

}

/// <summary>
/// 길이 구하기
/// </summary>
/// <returns>길이</returns>
float Vector3::Length() const
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

/// <summary>
/// 내적
/// </summary>
/// <param name="_other">벡터</param>
/// <returns>내적 값</returns>
float Vector3::Dot(const Vector3& _other) const
{
	return *this * _other;
}

/// <summary>
/// 일반화
/// </summary>
/// <returns>단위 벡터</returns>
void Vector3::Normalize()
{
	float norm = Length();
	this->x /= norm;
	this->y /= norm;
	this->z /= norm;
}

/// <summary>
/// 외적
/// </summary>
/// <param name="_other">벡터</param>
/// <returns>외적 벡터</returns>
Vector3 Vector3::Cross(const Vector3& _other)
{
	return Vector3{
		this->y * _other.z - this->z * _other.y,
		this->z * _other.x - this->x * _other.z,
		this->x * _other.y - this->y * _other.x
	};
}

Vector3 Vector3::operator+(const Vector3& _other) const
{
	Vector3 result;
	result.x = this->x + _other.x;
	result.y = this->y + _other.y;
	result.z = this->z + _other.z;
	return result;
}

void Vector3::operator*=(const float& _other)
{
	*this = *this * _other;
}

Vector3 Vector3::operator-(const Vector3& _other) const
{
	Vector3 result;
	result.x = this->x - _other.x;
	result.y = this->y - _other.y;
	result.z = this->z - _other.z;
	return result;
}

Vector3 Vector3::operator-()
{
	return Vector3{ -this->x, -this->y, -this->z };
}

Vector3 Vector3::operator-() const
{
	return Vector3{ -this->x, -this->y, -this->z };
}

bool Vector3::operator==(const Vector3& _other) const
{
	return this->x == _other.x && this->y == _other.y && this->z == _other.z;
}

float Vector3::operator*(const Vector3& _other) const
{
	return this->x * _other.x + this->y * _other.y + this->z * _other.z;
}

Vector3 Vector3::operator*(const float& _other) const
{
	return Vector3(this->x * _other, this->y * _other, this->z * _other);
}

Vector3 Vector3::operator/(const float& _other) const
{
	assert(_other != 0 && "devied 0");
	return Vector3(this->x / _other, this->y / _other, this->z / _other);

}

void Vector3::operator-=(const Vector3& _other)
{
	*this = *this - _other;
}

void Vector3::operator+=(const Vector3& _other)
{
	*this = *this + _other;
}
