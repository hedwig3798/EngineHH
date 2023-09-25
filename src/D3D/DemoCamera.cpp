#include "DemoCamera.h"

DemoCamera::DemoCamera(float _screenHight, float _screenWidth)
	: viewTM(DirectX::XMMatrixIdentity())
	, FOV(1.0f)
	, rotation{ 0, 0, 0 }
	, position{ 0, 0, 0 }
	, screenHight(_screenHight)
	, screenWidth(_screenWidth)
{

}

DemoCamera::~DemoCamera()
{

}

/// <summary>
/// 시야각 증감
/// </summary>
/// <param name="_value">증감 정도</param>
void DemoCamera::AddFOV(float _value)
{
	this->FOV += _value;
}

/// <summary>
/// 회전
/// </summary>
/// <param name="_value">각 축에 따른 회전 각</param>
void DemoCamera::Rotate(DirectX::XMFLOAT3 _value)
{
	this->rotation.x += _value.x;
	this->rotation.y += _value.y;
	this->rotation.z += _value.z;
}

/// <summary>
/// 이동
/// </summary>
/// <param name="_value">움직인 거리</param>
void DemoCamera::Traslation(DirectX::XMFLOAT3 _value)
{
	this->position.x += _value.x;
	this->position.y += _value.y;
	this->position.z += _value.z;
}

/// <summary>
/// 뷰 행렬 연산
/// </summary>
/// <returns>뷰 행렬</returns>
const DirectX::XMMATRIX& DemoCamera::GetViewTM()
{
	DirectX::XMMATRIX temp;
	temp = DirectX::XMMatrixRotationX(this->rotation.x);
	temp *= DirectX::XMMatrixRotationY(this->rotation.y);
	temp *= DirectX::XMMatrixRotationZ(this->rotation.z);
	temp *= DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(temp);

	this->viewTM = DirectX::XMMatrixInverse(&det, temp);

	return this->viewTM;
}

/// <summary>
/// 투영 행렬 연산
/// </summary>
/// <returns>투영 행렬</returns>
const DirectX::XMMATRIX& DemoCamera::GetProjectionTM()
{
	return this->projectionTM = DirectX::XMMatrixPerspectiveFovLH(this->FOV, this->screenWidth / this->screenHight, 1, 1000);;
}

void DemoCamera::Update()
{

}
