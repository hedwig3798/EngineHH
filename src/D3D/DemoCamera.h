#pragma once
#include "pch.h"

class DemoCamera
{
private:
	DirectX::XMMATRIX viewTM;
	DirectX::XMMATRIX projectionTM;

	float FOV;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 position;

	float screenHight;
	float screenWidth;

public:

	DemoCamera(float _screenHight, float _screenWidth);
	~DemoCamera();

	void AddFOV(float _value);
	void Rotate(DirectX::XMFLOAT3 _value);
	void Traslation(DirectX::XMFLOAT3 _value);

	const DirectX::XMMATRIX& GetViewTM();
	const DirectX::XMMATRIX& GetProjectionTM();

	void Update();
};

