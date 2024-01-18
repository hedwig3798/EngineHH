#pragma once
#include "ICamera.h"
#include <d3d11.h>
#include <DirectXMath.h>

class Camera 
	: public ICamera
{
public:
	DirectX::XMMATRIX viewTM;
	DirectX::XMMATRIX projectionTM;

	float FOV;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 position;

	DirectX::XMFLOAT3 dirRight;
	DirectX::XMFLOAT3 dirUp;
	DirectX::XMFLOAT3 dirLook;
public:
	Camera(float _screenHight, float _screenWidth);
	virtual ~Camera() override;

	void AddFOV(float _value) override;

	void Rotate(float _xValue, float _yValue, float _zValue) override;
	void Traslation(float _xValue, float _yValue, float _zValue) override;

	void MoveFoward(float _value) override;
	void MoveRight(float _value) override;
	void MoveUP(float _value) override;

	void RotateRight(float _value) override;
	void RotateUp(float _value) override;

	const DirectX::XMMATRIX GetViewTM();
	const DirectX::XMMATRIX GetProjectionTM();
	const DirectX::XMFLOAT3 GetPositoin();
};

