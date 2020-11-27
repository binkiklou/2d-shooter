#include "world.hpp"

#include "world_opcodes.hpp"

#include <iostream>

void world::start()
{
	this->running = true;

	this->m_thread = std::thread(&world::check_queue, this);
	
	this->last_tick = std::chrono::high_resolution_clock::now();
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

			// Well switch statements did some 
			// weird things, so this is a chain
			// of if statements now.
			if (m.opcode == WORLD_GET_OBJECTS)
			{
				filter();

				size_t opc = m.data.at(0);
				server* ptr = (server*)m.data.at(1);

				message request;
				request.opcode = opc;

				request.data.push_back((size_t)&this->objects);

				ptr->push_message(request);
			}
			else if (m.opcode == WORLD_CALLBACK_IF_CHANGED)
			{
				size_t opc = m.data.at(0);
				server* ptr = (server*)m.data.at(1);

				std::pair<size_t, server*> value;
				value.first = opc;
				value.second = ptr;

				this->change_cb.push_back(value);

				changed = true;
			}
			else if (m.opcode == WORLD_CALLBACK_ON_TICK)
			{
				size_t opc = m.data.at(0);
				server* ptr = (server*)m.data.at(1);

				std::pair<size_t, server*> value;
				value.first = opc;
				value.second = ptr;

				std::cout << "Pushed server to world callback on tick" << std::endl;
				this->tick_cb.push_back(value);
			}
			else if (m.opcode == WORLD_PUSH_OBJECT)
			{
				object* ptr = (object*)m.data.at(0);
				object tmp = *ptr;
				this->objects.push_back(tmp);
				this->objects.back().id = this->objects.size();

				changed = true;
			}
			else if (
				m.opcode == WORLD_PHYSICS_MOVE_OBJECT || 
				m.opcode == WORLD_PHYSICS_ROTATE_OBJECT
				)
			{
				p_queue.push(m);
				changed = true;
			}
			else if (m.opcode == WORLD_QUIT)
			{
				this->running = false;
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
			for (std::pair<size_t,server*> cb : this->change_cb)
			{
				if (cb.second != nullptr)
				{
					message m;
					m.opcode = cb.first;
					cb.second->push_message(m);
				}
			}
		}

		for (std::pair<size_t, server*> cb : this->tick_cb)
		{
			if (cb.second != nullptr)
			{
				message m;
				m.opcode = cb.first;
				cb.second->push_message(m);
			}
		}

		// why the fuck does chrono's namespaces
		// be so long
		auto now = std::chrono::high_resolution_clock::now();
		auto d = std::chrono::duration_cast
			<std::chrono::milliseconds>
			(now - this->last_tick).count();

		if (d > tickrate)
		{
			std::cout << "Ticktime is slow:" << d << std::endl;
		}

		if (d < (tickrate))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(tickrate - d));
			this->last_tick = std::chrono::high_resolution_clock::now();
		}
	}
}