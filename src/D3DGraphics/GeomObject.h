#pragma once
#include "pch.h"

class Mesh;
class GraphicsEngine;

class GeomObject
{
public:
	std::string name;

	std::vector<Mesh*> meshes;

	GeomObject* parent;
	std::vector<GeomObject*> children;

	bool isAnimation;
	int nowTick;
	float oneTick;
	float accTick;
	int maxTick;
	std::vector<std::pair<int, DirectX::XMFLOAT3>> animationPositionTrack;
	int positionTrackIndex;
	std::vector<std::pair<int, DirectX::XMFLOAT4>> animationRotateTrack;
	int rotateTrackIndex;

	DirectX::XMVECTOR localScale;
	DirectX::XMVECTOR localRotate;
	DirectX::XMVECTOR localPosition;

	DirectX::XMVECTOR nodeScale;
	DirectX::XMVECTOR nodeRotate;
	DirectX::XMVECTOR nodePosition;

public:
	bool isHelper;
	DirectX::XMMATRIX nodeTM;
	DirectX::XMMATRIX localTM;
	DirectX::XMMATRIX animationTM;

	bool isNegative;

	std::wstring path[2]
	{
		L"../Shader/VertexShader2.hlsl",
		L"../Shader/PixelShader2.hlsl",
	};

public:
	GeomObject();

	void AddMesh(Mesh* _mesh);
	void AddChild(GeomObject* _child);
	void SetParent(GeomObject* _parent);

	std::string GetName() const { return name; }
	void SetName(std::string val) { name = val; }

	void Render(GraphicsEngine* _graphicsEngine);

	void Initalize(GraphicsEngine* _graphicsEngine);

	void Localize(GraphicsEngine* _graphicsEngine);
	void SetLocal(bool _isLocal);

	void Update(float _dt);

	void Translate(float _x, float _y, float _z);
	void RoateBaseAxis(float _x, float _y, float _z);
	void Scale(float _x, float _y, float _z);

	void UpdateAnimation(float _dt);
};

