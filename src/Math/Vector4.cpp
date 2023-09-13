#include "Vector4.h"
#include "Vector3.h"

Vector4::Vector4(float _a /*= 0.f*/, float _b /*= 0.f*/, float _c /*= 0.f*/, float _d /*= 0.f*/)
	: x(_a)
	, y(_b)
	, z(_c)
	, w(_d)
{
}

Vector4::Vector4(Vector3 _vec3)
	: x(_vec3.x)
	, y(_vec3.y)
	, z(_vec3.z)
	, w(0)
{
}

Vector4::Vector4(Vector2 _vec2)
	: x(_vec2.x)
	, y(_vec2.y)
	, z(0)
	, w(0)
{
}

Vector4::~Vector4()
{
}

Vector4 Vector4::operator+(const Vector4& _other) const
{
	return Vector4(this->x + _other.x, this->y + _other.y, this->z + _other.z, this->w + _other.w);
}

void Vector4::operator*=(const float& _other)
{
	*this = *this * _other;
}

Vector4 Vector4::operator-(const Vector4& _other) const
{
	return Vector4(this->x - _other.x, this->y - _other.y, this->z - _other.z, this->w - _other.w);
}

Vector4 Vector4::operator-()
{
	return Vector4(-this->x, -this->y, -this->z, -this->w);
}

Vector4 Vector4::operator-() const
{
	return Vector4(-this->x, -this->y, -this->z, -this->w);
}

float Vector4::Length() const
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

float Vector4::Dot(const Vector4& _other) const
{
	return *this * _other;
}

void Vector4::Normalize()
{
	float norm = this->Length();

	this->x /= norm;
	this->y /= norm;
	this->z /= norm;
	this->w /= norm;
}

bool Vector4::operator==(const Vector4& _other) const
{
	return this->x == _other.x && this->y == _other.y && this->z == _other.z && this->w == _other.w;
}

float Vector4::operator*(const Vector4& _other) const
{
	return this->x * _other.x + this->y * _other.y + this->z * _other.z + this->w * _other.w;
}

Vector4 Vector4::operator*(const float& _other) const
{
	return Vector4(this->x * _other, this->y * _other, this->z * _other, this->w * _other);
}

Vector4 Vector4::operator/(const float& _other) const
{
	assert(_other != 0 && "devied 0");
	return Vector4(this->x / _other, this->y / _other, this->z / _other, this->w / _other);
}

void Vector4::operator-=(const Vector4& _other)
{
	*this = *this - _other;
}

void Vector4::operator+=(const Vector4& _other)
{
	*this = *this + _other;
}

