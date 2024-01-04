#pragma once
#include "pch.h"
#include "pipeline.h"
#include "LightHelper.h"


class FbxData;
class GraphicsEngine;
class FbxMeshData;

namespace VertexF
{
	struct Data;
};

class FObject
{
public:
	FbxData* fData;
	Material demoMat;

private:
	GraphicsEngine* gp;
	std::wstring texturePath;
	std::wstring* path;

public:
	FObject();
	~FObject();

	void Render(GraphicsEngine* _gp, DirectX::XMMATRIX _viewTM, DirectX::XMMATRIX _projTM);
	void Initalize(GraphicsEngine* gp, std::wstring _sPath[], std::wstring _texturePath, FbxData* _nowData);

private:
	void CreatePipeline(FbxMeshData* _nowMesh);
};

