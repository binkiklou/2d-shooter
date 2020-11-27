#pragma once

#include "object.hpp"

#include <SFML/Graphics.hpp>

class ray
{
public:
	vector2 direction;

	float distance;
	bool hit = false;

	// Color
	int r = 0;
	int g = 0;
	int b = 0;
};

// Improvement: Make this multithreaded
class raycaster
{
public:
	void raycast(int);
	void reset(int);

	bool on_segment(vector2, vector2, vector2);
	int orientation(vector2, vector2, vector2);

	std::vector<ray> rays;
	std::vector<ray> clean;

	std::vector<object> local_objects;
};