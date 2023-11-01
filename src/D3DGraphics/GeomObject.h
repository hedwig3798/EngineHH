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

public:
	bool isHelper;
	DirectX::XMMATRIX nodeTM;
	DirectX::XMMATRIX localTM;

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
};

