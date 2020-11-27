#pragma once

#include "world.hpp"

class script
{
public:
	virtual void start();
	virtual void update();
	world* w_ptr;
};