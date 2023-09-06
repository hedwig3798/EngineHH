#pragma once
#include "pch.h"

class Object
{
protected:
	std::string name;
	unsigned int ID = 0;

public:
	Object() {};
	virtual ~Object() {};
};

