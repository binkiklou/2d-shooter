#include "raycast.hpp"

#include <cmath>
#include <string>
#include <iostream>
#include <chrono>

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

				vector2 p = (r.direction * i) + c_pos;

				for (const object& obj : this->local_objects)
				{
					if (obj.id != 0)
					{
						for (const vector2& q : obj.tiles)
						{
							if (
								p.x >= (q.x - 1) &&
								p.y >= (q.y - 1) &&
								p.x <= (q.x + 1) &&
								p.y <= (q.y + 1)
								)
							{
								r.distance = std::sqrtf( std::powf(c_pos.x - p.x,2) + std::powf(c_pos.y - p.y,2) );
								//r.distance = std::sqrtf( std::powf() );
								//std::cout << r.distance << std::endl;
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
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	if (d > 60)
	{
		std::cout << "Raycasting is slow: " << d << std::endl;
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
			r.angle = ((x * d) + a) * (3.1415 / 180);
			r.direction = vector2(
				std::cosf( r.angle ),
				std::sinf( r.angle )
			);
			this->clean.push_back(r);
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	this->rays = clean;
}