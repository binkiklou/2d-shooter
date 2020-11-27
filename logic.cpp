#include "logic.hpp"

#include "logic_opcodes.hpp"

#include <iostream>

logic::logic()
{
	this->w_ptr = nullptr;
}

logic::logic(world* w)
{
	this->w_ptr = w;
}

void logic::start()
{
	this->running = true;
	this->m_thread = std::thread(&logic::run_logic,this);
}

void logic::run_logic()
{
	std::cout<<"Logic server started" << std::endl;

	while (this->running)
	{
		this->mtx.lock();
		while (!this->m_queue.empty())
		{
			message m = m_queue.front();

			switch (m.opcode)
			{
			case LOGIC_PUSH_SCRIPT:
				push_script(m);
				break;
			case LOGIC_GET_WORLD_PTR:
			{
				world* ptr = (world*)m.data.at(0);
				ptr = w_ptr;
			}
				break;
			}

			m_queue.pop();
		}

		for (script* s : this->scripts)
		{
			if (s != nullptr)
			{
				s->update();
			}
			// Improvement: Destroy the script if it's null
		}
		this->mtx.unlock();
	}
}

void logic::push_script(message m)
{
	size_t addr = m.data.at(0);
	script* s = (script*)addr;
	this->scripts.push_back(s);
	this->scripts.back()->w_ptr = this->w_ptr;
	this->scripts.back()->start();
}