#include "raycast.hpp"

#include <cmath>
#include <string>
#include <iostream>

// Modified from geeksforgeeks
//	https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool raycaster::on_segment(vector2 p,vector2 q,vector2 r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

// Modified from geeksforgeeks
//	https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
int raycaster::orientation(
	vector2 p,
	vector2 q,
	vector2 r
)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0;  // colinear 
	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

void raycaster::raycast(int limit)
{
	for (ray& r : this->rays)
	{
		if (!r.hit)
		{
			for (int i = 0; i < limit; i++)
			{
				if (r.hit)
					break;

				vector2 p1 = r.direction * (i - 1);
				vector2 q1 = r.direction * i;

				for (const object& obj : this->local_objects)
				{
					vector2 p2 = obj.coords.first;
					p2 = p2 + obj.pos;
					vector2 q2 = obj.coords.second;
					q2 = q2 + obj.pos;

					int o1 = this->orientation(p1,q1,p2);
					int o2 = this->orientation(p1,q1,q2);
					int o3 = this->orientation(p2,q2,p1);
					int o4 = this->orientation(p2,q2,q1);

					if(o1 != o2 && o3 != o4)
					{
						std::cout << "Ray Intersected with wall " << i << std::endl;
						r.distance = i;
						r.hit = true;

						r.r = 255;
						r.g = 255;
						r.b = 255;
						break;
					}
				}
			}
		}
	}
}

void raycaster::reset(int width)
{
	if (this->clean.empty() || this->clean.size() != width)
	{
		// FOV is hardcoded rn
		float d = (float)80 / width;
		for (int x = -40; x < width; x++)
		{
			ray r;
			r.hit = false;
			r.direction = vector2(
				std::cosf( (x * d) * (3.1415/180) ),
				std::sinf( (x * d) * (3.1415/180) )
			);
			this->clean.push_back(r);
		}
	}
	this->rays = clean;
}