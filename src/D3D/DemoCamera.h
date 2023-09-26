#pragma once
#include "pch.h"

class ManagerSet;

class DemoCamera
{
private:
	DirectX::XMMATRIX viewTM;
	DirectX::XMMATRIX projectionTM;

	float FOV;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 position;

	DirectX::XMFLOAT3 dirRight;
	DirectX::XMFLOAT3 dirUp;
	DirectX::XMFLOAT3 dirLook;

	float screenHight;
	float screenWidth;

	ManagerSet* managers;

public:

	DemoCamera(float _screenHight, float _screenWidth, ManagerSet* _managers);
	~DemoCamera();

	void AddFOV(float _value);
	void Rotate(DirectX::XMFLOAT3 _value);
	void Traslation(DirectX::XMFLOAT3 _value);

	void MoveFoward(float _value);
	void MoveRight(float _value);
	void MoveUP(float _value);

	void RotateRight(float _value);
	void RotateUp(float _value);

	const DirectX::XMMATRIX& GetViewTM();
	const DirectX::XMMATRIX& GetProjectionTM();

	void Update();
};

