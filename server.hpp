#pragma once

#include "message.hpp"

#include <thread>
#include <mutex>
#include <queue>

// Server, makes it so you can pass messages
// between different threads
class server
{
public:
	// Starts the server's main thread
	virtual void start();

	// Stops the server's entire thread
	// (with questionable safety)
	virtual void stop();

	// Returns a bool, I guess
	bool is_ok();

	// Push a message to the queue
	// That will be processed in a different thread.
	void push_message(message);

protected:
	bool running = false;

	std::mutex mtx;
	std::thread m_thread;
	std::queue<message> m_queue;
};