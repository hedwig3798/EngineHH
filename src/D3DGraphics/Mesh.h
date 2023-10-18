#pragma once
#include "pch.h"

class Mesh
{
public:
	std::vector<VertexT::Data> vertexes;
	std::vector<UINT> indexes;

public:
	Mesh();
	~Mesh();
};

