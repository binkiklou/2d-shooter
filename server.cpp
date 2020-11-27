#include "server.hpp"

#include <iostream>

void server::start()
{
	this->running = true;
	std::cout<<"Default server starting"<<std::endl;
}

bool server::is_ok()
{
	return this->running;
}

void server::stop()
{
	// Just if it already wasn't
	this->running = false;
	std::cout << "Trying to kill thread " << this->m_thread.get_id() << std::endl;
	this->m_thread.join();
}

void server::push_message(message m)
{
	this->mtx.lock();

	m_queue.push(m);

	this->mtx.unlock();
}