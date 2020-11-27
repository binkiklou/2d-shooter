#pragma once

#include "transforms.hpp"

#include <vector>

class object
{
public:
	object();

	// Transforms
	vector2 pos;
	int angle = 0;
	std::vector<vector2> tiles;

	int id = -1;
};

class camera : public object
{
public:
	int fov = 80;
};