#pragma once
#include "Object.h"

class Component
	: public Object
{
protected:
	TAG tag = TAG::PLAYER;

public:
	Component() {};
	virtual ~Component() {};
};

