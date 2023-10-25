#pragma once
#include "pch.h"

class Mesh;
class GraphicsEngine;

class GeomObject
{
private:
	std::string name;
	std::string parentname;

	bool isHelper;

	std::vector<Mesh*> meshes;

public:
	DirectX::XMMATRIX nodeTM;
	DirectX::XMMATRIX inheritTM;

	std::wstring path[2]
	{
		L"../Shader/VertexShader2.hlsl",
		L"../Shader/PixelShader2.hlsl",
	};

public:
	GeomObject();

	void AddMesh(Mesh* _mesh);
	std::string GetName() const { return name; }
	void SetName(std::string val) { name = val; }

	void Render(GraphicsEngine* _graphicsEngine);

	void Initalize(GraphicsEngine* _graphicsEngine);
};

