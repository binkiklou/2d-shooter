#include "raycast.hpp"

#include <cmath>
#include <string>
#include <iostream>
#include <chrono>

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
	auto t1 = std::chrono::high_resolution_clock::now();
	for (ray& r : this->rays)
	{
		if (!r.hit && local_objects.size() > 0)
		{
			// Camera will always be the object with id 0
			vector2 c_pos = local_objects.at(0).pos;

			for (int i = 0; i < limit; i++)
			{
				if (r.hit)
					break;

				vector2 p1 = c_pos + (r.direction * (i - 1));
				vector2 q1 = c_pos + (r.direction * i);

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
					//	std::cout << "Ray Intersected with wall " << i << std::endl;
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
	auto t2 = std::chrono::high_resolution_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	if (d > 60)
	{
		std::cout << "Raycasting slow: " << d << std::endl;
	}
	//std::cout << "Raycasting took:" << d << std::endl;
}

void raycaster::reset(int width,int a)
{
	auto t1 = std::chrono::high_resolution_clock::now();
	if (this->clean.empty() ||
		this->last_width != width || 
		this->last_angle != a)
	{
		this->clean.clear();
		float d = (float)80 / width;
		for (int x = -(width/2); x < (width/2); x++)
		{
			ray r;
			r.hit = false;
			r.direction = vector2(
				std::cosf( ((x * d) + a) * (3.1415/180) ),
				std::sinf( ((x * d) + a)* (3.1415/180) )
			);
			this->clean.push_back(r);
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	this->rays = clean;
}