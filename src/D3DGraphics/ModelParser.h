#pragma once
#include "pch.h"
#include "Vertex.h"
#include "Mesh.h"

void GetVertexAndIndex(std::vector<VertexC::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath);
void GetVertexAndIndex(std::vector<VertexT::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath);
std::vector<Mesh*> AseParser(std::wstring _filePath);

std::vector<std::string> split(std::string& input, char delimiter);
std::vector<std::string> ASEToken(std::string& _input);

