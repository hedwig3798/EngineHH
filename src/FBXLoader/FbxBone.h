#pragma once
#include "pch.h"

class FbxBone
{
public:
	FbxBone* parent;
	std::vector<FbxBone*> child;
	UINT index;

	FbxBone();
	~FbxBone();
};

