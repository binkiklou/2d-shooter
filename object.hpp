#pragma once

#include "transforms.hpp"

#include <utility>

class object
{
public:
	object();

	vector2 pos;

	float angle = 0;

	std::pair<vector2, vector2> coords;
};

class camera : public object
{
public:
	int fov = 80;
};