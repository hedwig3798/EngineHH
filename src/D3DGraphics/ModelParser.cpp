#include "ModelParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "color.h"
#include "ASEFile.h"
void GetVertexAndIndex(std::vector<VertexC::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath)
{
	std::string line;
	std::ifstream file(_filePath);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::vector<std::string> parsed = split(line, ' ');

			if (parsed[0] == "v")
			{
				VertexC::Data input = { DirectX::XMFLOAT3{std::stof(parsed[1]) / 10.0f, std::stof(parsed[2]) / 10.0f ,std::stof(parsed[3]) / 10.0f}, COLORS::White };
				_vertexes.push_back(input);
			}
			else if (parsed[0] == "f")
			{
				for (int i = 1; i < parsed.size(); i++)
				{
					std::vector<std::string> indexData = split(parsed[i], '/');
					_indexes.push_back((UINT)(std::stoi(indexData[0])));
				}
			}
		}
	}
	else
	{
		assert(false && "cannot read 3d model object");
	}
}

std::vector<Mesh*> AseParser(std::wstring _filePath)
{
	std::string line;
	std::ifstream file(_filePath);

	std::vector<Mesh*> result;
	std::vector<std::string> s;
	DirectX::XMMATRIX LTM = DirectX::XMMatrixIdentity();

	int optimizeSize = 0;
	int optimizeIndex = 0;

	Mesh* nowMesh = nullptr;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			s.clear();
			ASEToken(line, s);

			if (s[0] == Token[_ASEToken::TOKENR_MESH])
			{
				result.push_back(new Mesh());
				nowMesh = result.back();
				optimizeIndex = 0;
				optimizeSize = 0;
			}
			else if (s[0] == Token[_ASEToken::TOKENR_MESH_NUMVERTEX])
			{
				assert(nowMesh && "Ase parser error. no mesh in data");
				nowMesh->origianlVertexList = std::vector<VertexT::Data>(std::stoi(s[1]), VertexT::Data());
			}
			else if (s[0] == Token[_ASEToken::TOKENR_MESH_NUMFACES])
			{
				assert(nowMesh && "Ase parser error. no mesh in data");
				optimizeSize = std::stoi(s[1]) * 3;
				nowMesh->indexList = std::vector<UINT>(optimizeSize);
				nowMesh->optimizeVertexList = std::vector<VertexT::Data>(optimizeSize);
				// result.back()->optimizeVertexList = std::vector<VertexT::Data>(std::stoi(s[1]) * 3);
			}
			else if (s[0] == Token[_ASEToken::TOKENR_MESH_VERTEX])
			{
				assert(nowMesh && "Ase parser error. no mesh in data");
				nowMesh->origianlVertexList[std::stoi(s[1])].position = DirectX::XMFLOAT3{ std::stof(s[2]) * 2, std::stof(s[4]) * 2, std::stof(s[3]) * 2 };
			}
			else if (s[0] == Token[_ASEToken::TOKENR_MESH_FACE])
			{
// 				int startIndex = std::stoi(s[1]) * 3;
// 				result.back()->indexList[startIndex] = std::stoi(s[3]);
// 				result.back()->indexList[startIndex + 2] = std::stoi(s[3 + 2]);
// 				result.back()->indexList[startIndex + 1] = std::stoi(s[3 + 2 + 2]);
			}
			else if (s[0] == Token[_ASEToken::TOKENR_MESH_VERTEXNORMAL])
			{
				assert(nowMesh && "Ase parser error. no mesh in data");
				VertexT::Data input = { nowMesh->origianlVertexList[std::stoi(s[1])].position,  DirectX::XMFLOAT3{ std::stof(s[2]), std::stof(s[3]), std::stof(s[4]) }, {} };
				nowMesh->optimizeVertexList[optimizeIndex++] = input;
			}
		}
	}
	else
	{
		assert(false && "cannot read 3d model object");
	}
	return result;
}

std::vector<std::string> split(std::string& input, char delimiter)
{
	std::vector<std::string> answer;
	std::stringstream ss(input);
	std::string temp;

	while (getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}

void ASEToken(std::string& _input, std::vector<std::string>& _tokens)
{
	std::string temp = "";

	for (auto& c : _input)
	{
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != ':')
		{
			temp += c;
		}
		else
		{
			if (temp != "")
			{
				_tokens.push_back(temp);
			}
			temp = "";
		}
	}
	_tokens.push_back(temp);
}
