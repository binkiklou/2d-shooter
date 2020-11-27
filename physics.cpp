#include "physics.hpp"

#include "world_opcodes.hpp"

#include <iostream>

void world::move(int id, vector2 dir)
{
	// No collision checking yet
	this->objects.at(id).pos = this->objects.at(id).pos + dir;
	//std::cout << "Moving object" << std::endl;
}

void world::update_physics(std::queue<message> p_queue)
{
	this->filter();

	while (!p_queue.empty())
	{
		message m = p_queue.front();

		switch (m.opcode)
		{
		case WORLD_PHYSICS_MOVE_OBJECT:
		{
			int id = (int)m.data.at(0);
			vector2* ptr = (vector2*)m.data.at(1);
			vector2 p = *ptr;
			move(id,p);
			break;
		}
		case WORLD_PHYSICS_ROTATE_OBJECT:
		{
			int id = (int)m.data.at(0);
			int* ptr = (int*)m.data.at(1);
			this->objects.at(id).angle = *ptr;
			//std::cout << this->objects.at(id).angle << std::endl;
			break;
		}
		}

		p_queue.pop();
	}
}
