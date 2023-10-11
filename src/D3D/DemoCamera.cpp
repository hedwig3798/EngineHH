#include "DemoCamera.h"
#include "ManagerSet.h"

DemoCamera::DemoCamera(float _screenHight, float _screenWidth, ManagerSet* _managers)
	: viewTM(DirectX::XMMatrixIdentity())
	, projectionTM(DirectX::XMMatrixIdentity())
	, FOV(1.0f)
	, rotation{ 0, 0, 0 }
	, position{ 0, 0, 0 }
	, screenHight(_screenHight)
	, screenWidth(_screenWidth)
	, managers(_managers)
	, dirUp{ 0.0f, 1.0f, 0.0f }
	, dirRight{ 1.0f, 0.0f, 0.0f }
	, dirLook{ 0.0f, 0.0f, 1.0f }

{
	
	Traslation(DirectX::XMFLOAT3{ -10.0f, 10.0f, 10.0f });
	Rotate(DirectX::XMFLOAT3{ 0.7, 2.3, 0 });
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
	RotateUp(_value.x);
	RotateRight(_value.y);
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
/// 전방 이동
/// </summary>
/// <param name="_value">이동 값</param>
void DemoCamera::MoveFoward(float _value)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(_value);
	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&this->dirLook);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&this->position);
	DirectX::XMStoreFloat3(&this->position, DirectX::XMVectorMultiplyAdd(s, l, p));
}

/// <summary>
/// 측면 이동
/// </summary>
/// <param name="_value">이동 값</param>
void DemoCamera::MoveRight(float _value)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(_value);
	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&this->dirRight);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&this->position);
	DirectX::XMStoreFloat3(&this->position, DirectX::XMVectorMultiplyAdd(s, r, p));
}

/// <summary>
/// 상하 이동
/// </summary>
/// <param name="_value">이동 값</param>
void DemoCamera::MoveUP(float _value)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(_value);
	DirectX::XMVECTOR u = DirectX::XMLoadFloat3(&this->dirUp);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&this->position);
	DirectX::XMStoreFloat3(&this->position, DirectX::XMVectorMultiplyAdd(s, u, p));
}

void DemoCamera::RotateRight(float _value)
{
	this->rotation.y += _value;
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(_value);

	DirectX::XMStoreFloat3(&this->dirRight, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirRight), R));
	DirectX::XMStoreFloat3(&this->dirUp, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirUp), R));
	DirectX::XMStoreFloat3(&this->dirLook, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirLook), R));
}

void DemoCamera::RotateUp(float _value)
{
	this->rotation.x += _value;
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&this->dirRight), _value);

	DirectX::XMStoreFloat3(&this->dirUp, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirUp), R));
	DirectX::XMStoreFloat3(&this->dirLook, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->dirLook), R));
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
	// this->viewTM = temp;
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

DirectX::XMFLOAT3 DemoCamera::GetPosition()
{
	return position;
}

/// <summary>
/// 카메라의 이동
/// </summary>
void DemoCamera::Update()
{
	if (this->managers->keyManager->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		RotateRight(this->managers->timeManager->GetfDT());
	}
	if (this->managers->keyManager->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		RotateRight(-this->managers->timeManager->GetfDT());
	}
	if (this->managers->keyManager->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		RotateUp(-this->managers->timeManager->GetfDT());
	}
	if (this->managers->keyManager->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
	{
		RotateUp(this->managers->timeManager->GetfDT());
	}

	if (this->managers->keyManager->GetKeyState(KEY::W) == KEY_STATE::HOLD)
	{
		MoveFoward(this->managers->timeManager->GetfDT() * 10);
	}
	if (this->managers->keyManager->GetKeyState(KEY::S) == KEY_STATE::HOLD)
	{
		MoveFoward(-this->managers->timeManager->GetfDT() * 10);
	}
	if (this->managers->keyManager->GetKeyState(KEY::A) == KEY_STATE::HOLD)
	{
		MoveRight(-this->managers->timeManager->GetfDT() * 10);
	}
	if (this->managers->keyManager->GetKeyState(KEY::D) == KEY_STATE::HOLD)
	{
		MoveRight(this->managers->timeManager->GetfDT() * 10);
	}
	if (this->managers->keyManager->GetKeyState(KEY::Q) == KEY_STATE::HOLD)
	{
		MoveUP(-this->managers->timeManager->GetfDT() * 10);
	}
	if (this->managers->keyManager->GetKeyState(KEY::E) == KEY_STATE::HOLD)
	{
		MoveUP(this->managers->timeManager->GetfDT() * 10);
	}
}
