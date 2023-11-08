#pragma once
#include "pch.h"
#include "Vertex.h"
#include "pipeline.h"

class GraphicsEngine;
class RenderObject;

class Mesh
{
public:
	std::vector<VertexT::Data> vertexList;
	std::vector<UINT> indexList;
	PipeLine pipeline;

	VertexT::Data* worldVertexes;
	VertexT::Data* localVertexes;
	UINT* indexes;

	bool isLocal;

	std::vector<RenderObject*> bones;
	std::vector<std::string> boneNames;

public:
	Mesh();
	~Mesh();

	void Render(GraphicsEngine* gp, const DirectX::XMMATRIX& _viewTM, const DirectX::XMMATRIX& _projTM);
	void CreatePipeline(GraphicsEngine* gp, std::wstring _sPath[], std::wstring _texturePath);

	void ChangeVertex(GraphicsEngine* gp);
};

