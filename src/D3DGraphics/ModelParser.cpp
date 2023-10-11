#include "ModelParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "color.h"

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

void TestParser(std::vector<VertexT::Data>& _vertexes, std::vector<UINT>& _indexes, std::wstring _filePath)
{
	std::string line;
	std::ifstream file(_filePath);

	DirectX::XMMATRIX LTM = DirectX::XMMatrixIdentity();

	int step = 0;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::vector<std::string> parsed = split(line, ' ');
			switch (step)
			{
			case 0:
			{
				DirectX::XMFLOAT3 rotation = { std::stof(parsed[0]), std::stof(parsed[1]) ,std::stof(parsed[2]) };
				LTM = DirectX::XMMatrixRotationX(rotation.x);
				LTM *= DirectX::XMMatrixRotationY(rotation.y);
				LTM *= DirectX::XMMatrixRotationZ(rotation.z);
				step++;
				break;
			}
			case 1:
			{
				for (int i = 0; i < parsed.size(); i++)
				{
					_indexes.push_back((UINT)(std::stoi(parsed[i])));
				}
				step++;
				break;
			}
			case 2:
			{
				for (int i = 0; i < parsed.size(); i += 3)
				{
					VertexT::Data input = { DirectX::XMFLOAT3{std::stof(parsed[i]) / 10.0f, std::stof(parsed[i + 1]) / 10.0f ,std::stof(parsed[i + 2]) / 10.0f}, {} };
					_vertexes.push_back(input);
				}
				step++;
				break;
			}
			case 3:
			{
				for (int i = 0; i < parsed.size(); i += 3)
				{
					_vertexes[i / 3].normal.x = std::stof(parsed[i]);
					_vertexes[i / 3].normal.y = std::stof(parsed[i + 1]);
					_vertexes[i / 3].normal.z = std::stof(parsed[i + 2]);
				}
				step++;
				break;
			}
			case 4:
			{
				for (int i = 0; i < parsed.size(); i += 2)
				{
					_vertexes[i / 2].texture.x = std::stof(parsed[i]);
					_vertexes[i / 2].texture.y = 1 - std::stof(parsed[i + 1]);
				}
				step++;
				break;
			}
			}

		}
	}
	else
	{
		assert(false && "cannot read 3d model object");
	}
}

std::vector<std::string> split(std::string input, char delimiter)
{
	std::vector<std::string> answer;
	std::stringstream ss(input);
	std::string temp;

	while (getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}