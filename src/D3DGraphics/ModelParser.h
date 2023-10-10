#pragma once
#include "pch.h"
#include "Vertex.h"

void GetVertexAndIndex(std::vector<VertexC::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath);
void GetVertexAndIndex(std::vector<VertexT::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath);
void TestParser(std::vector<VertexT::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath);

std::vector<std::string> split(std::string input, char delimiter);

