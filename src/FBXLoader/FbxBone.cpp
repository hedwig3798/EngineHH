#include "FbxBone.h"

FbxBone::FbxBone()
	: parent(nullptr)
	, child(std::vector<FbxBone*>())
	, index(-1)
{

}

FbxBone::~FbxBone()
{

}
