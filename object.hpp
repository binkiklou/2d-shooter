#pragma once

#include "transforms.hpp"

#include <utility>

class object
{
public:
	object();

	// Transforms
	vector2 pos;
	int angle = 0;
	std::pair<vector2, vector2> coords;

	int id = -1;
};

class camera : public object
{
public:
	int fov = 80;
};