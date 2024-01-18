#include "Camera.h"

Camera::Camera(float _screenHight, float _screenWidth)
	:ICamera(_screenHight, _screenWidth)
	, viewTM(DirectX::XMMatrixIdentity())
	, projectionTM(DirectX::XMMatrixIdentity())
	, FOV(1.0f)
	, rotation{ 0, 0, 0 }
	, position{ 0, 0, 0 }
	, dirUp{ 0.0f, 1.0f, 0.0f }
	, dirRight{ 1.0f, 0.0f, 0.0f }
	, dirLook{ 0.0f, 0.0f, 1.0f }
{
	Traslation(-10.0f, 10.0f, 10.0f );
	Rotate( 0.7f, 2.3f, 0.f );
}

Camera::~Camera()
{
}

void Camera::AddFOV(float _value)
{
	this->FOV += _value;
}

void Camera::Rotate(float _xValue, float _yValue, float _zValue)
{
	RotateUp(_xValue);
	RotateRight(_yValue);
	this->rotation.z += _zValue;
}

void Camera::Traslation(float _xValue, float _yValue, float _zValue)
{
	this->position.x += _xValue;
	this->position.y += _yValue;
	this->position.z += _zValue;
}

void Camera::MoveFoward(float _value)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(_value);
	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&this->dirLook);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&this->position);
	DirectX::XMStoreFloat3(&this->position, DirectX::XMVectorMultiplyAdd(s, l, p));
}

void Camera::MoveRight(float _value)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(_value);
	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&this->dirRight);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&this->position);
	DirectX::XMStoreFloat3(&this->position, DirectX::XMVectorMultiplyAdd(s, r, p));
}

void Camera::MoveUP(float _value)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(_value);
	DirectX::XMVECTOR u = DirectX::XMLoadFloat3(&this->dirUp);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&this->position);
	DirectX::XMStoreFloat3(&this->position, DirectX::XMVectorMultiplyAdd(s, u, p));
}

void Camera::RotateRight(float _value)
{
	this->rotation.y += _value;
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(_value);

	DirectX::XMStoreFloat3(&this->dirRight, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirRight), R));
	DirectX::XMStoreFloat3(&this->dirUp, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirUp), R));
	DirectX::XMStoreFloat3(&this->dirLook, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirLook), R));
}

void Camera::RotateUp(float _value)
{
	this->rotation.x += _value;
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&this->dirRight), _value);

	DirectX::XMStoreFloat3(&this->dirUp, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirUp), R));
	DirectX::XMStoreFloat3(&this->dirLook, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirLook), R));
}

const DirectX::XMMATRIX Camera::GetViewTM()
{
	DirectX::XMMATRIX temp;
	temp = DirectX::XMMatrixRotationX(this->rotation.x);
	temp *= DirectX::XMMatrixRotationY(this->rotation.y);
	temp *= DirectX::XMMatrixRotationZ(this->rotation.z);
	temp *= DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(temp);

	this->viewTM = DirectX::XMMatrixInverse(&det, temp);
	// this->viewTM = temp;
	return this->viewTM;
}

const DirectX::XMMATRIX Camera::GetProjectionTM()
{
	return this->projectionTM = DirectX::XMMatrixPerspectiveFovLH(this->FOV, this->screenWidth / this->screenHight, 1, 1000);;
}

const DirectX::XMFLOAT3 Camera::GetPositoin()
{
	return position;
}
