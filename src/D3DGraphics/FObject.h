#pragma once
#include <string>
class FbxData;
class GraphicsEngine;
class IGraphicsEngine;
class FbxMeshData;

namespace VertexF
{
	struct Data;
};

class FObject
{
public:
	FbxData* fData;

private:
	GraphicsEngine* gp;
	std::wstring texturePath;
	std::wstring* path;

public:
	FObject();
	~FObject();

	void Render();
	void Initalize(IGraphicsEngine* gp, std::wstring _sPath[], std::wstring _texturePath, FbxData* _nowData);

private:
	void CreatePipeline(FbxMeshData* _nowMesh);
	void ReleasePipeline(FbxMeshData* _nowMesh);
};

