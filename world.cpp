#include "world.hpp"

#include "world_opcodes.hpp"

#include <chrono>
#include <iostream>

void world::start()
{
	this->running = true;
	this->m_thread = std::thread(&world::check_queue, this);
}

void world::check_queue()
{
	std::cout << "World server running" << std::endl;

	while (this->running)
	{
		bool changed = false;
		short tickdelay = (1000 / this->tickrate);

		if (!this->m_queue.empty())
			changed = true;

		this->mtx.lock();
		while (!this->m_queue.empty())
		{
			message m = m_queue.front();

			switch (m.opcode)
			{
			case WORLD_GET_OBJECTS:
			{
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
			}
				break;

			case WORLD_PUSH_OBJECT:
			{
				object* ptr = (object*)m.data.at(0);
				object tmp = *ptr;
				this->objects.push_back(tmp);
			}
				break;
			}

			this->m_queue.pop();
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