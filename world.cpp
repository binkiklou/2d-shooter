#include "world.hpp"

#include "world_opcodes.hpp"

#include <chrono>
#include <iostream>

void world::start()
{
	this->running = true;

	this->m_thread = std::thread(&world::check_queue, this);
}

void world::filter()
{
	int i = 0;
	for (object& obj : this->objects)
	{
		obj.id = i;
		i++;
	}
}

void world::check_queue()
{
	std::cout << "World server running" << std::endl;

	camera c;
	c.fov = 80;
	c.id = 0;
	this->objects.push_back(c);

	while (this->running)
	{
		bool changed = false;
		short tickdelay = (1000 / this->tickrate);

		this->mtx.lock();
		
		std::queue<message> p_queue;

		while (!this->m_queue.empty())
		{
			message m = m_queue.front();

			switch (m.opcode)
			{
			case WORLD_GET_OBJECTS:
			{
				filter();

				//std::cout << "Request for object vector" << std::endl;
				//std::cout << "Size is: " << this->objects.size() << std::endl;

				size_t opc = m.data.at(0);
				server* ptr = (server*)m.data.at(1);

				message request;
				request.opcode = opc;
				
				request.data.push_back((size_t)&this->objects);

				ptr->push_message(request);
			}
				break;

			case WORLD_CALLBACK_IF_CHANGED:
			{
				size_t opc = m.data.at(0);
				server* ptr = (server*)m.data.at(1);

				std::pair<size_t, server*> value;
				value.first = opc;
				value.second = ptr;

				this->callbacks.push_back(value);

				changed = true;
			}
				break;

			case WORLD_PUSH_OBJECT:
			{
				object* ptr = (object*)m.data.at(0);
				object tmp = *ptr;
				this->objects.push_back(tmp);
				this->objects.back().id = this->objects.size();

				changed = true;
			}
				break;
				// PHYSICS IS ALWAYS DONE LAST
			case WORLD_PHYSICS_MOVE_OBJECT:
				p_queue.push(m);
				changed = true;
				break;

			case WORLD_PHYSICS_ROTATE_OBJECT:
				p_queue.push(m);
				changed = true;
				break;

			case WORLD_QUIT:
				this->running = false;
				break;
			}

			this->m_queue.pop();
		}

		if (!p_queue.empty())
		{
			this->update_physics(p_queue);
		}

		this->mtx.unlock();

		if (changed)
		{
			for (std::pair<size_t,server*> cb : this->callbacks)
			{
				if (cb.second != nullptr)
				{
					message m;
					m.opcode = cb.first;
					cb.second->push_message(m);
				}
			}
		}

		// IMPROVEMENT: Make it so it just sleeps
		// the remaining time, until next tick.
		std::this_thread::sleep_for(
			std::chrono::milliseconds(tickdelay)
		);
	}
}