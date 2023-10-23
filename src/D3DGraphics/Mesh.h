#pragma once
#include "pch.h"
#include "Vertex.h"
#include "pipeline.h"
class GraphicsEngine;

class Mesh
{
public:
	std::vector<VertexT::Data> origianlVertexList;
	std::vector<VertexT::Data> optimizeVertexList;
	std::vector<UINT> indexList;
	PipeLine pipeline;

	VertexT::Data* vertexes;
	UINT* indexes;

public:
	Mesh();
	~Mesh();
	void Render(GraphicsEngine* gp);
	void CreatePipeline(GraphicsEngine* gp, std::wstring _sPath[], std::wstring _texturePath);
};

