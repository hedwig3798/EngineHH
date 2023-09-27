#pragma once
#include "pch.h"
#include "Vertex.h"

void GetVertexAndIndex(std::vector<Vertex>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath);

std::vector<std::string> split(std::string input, char delimiter);

