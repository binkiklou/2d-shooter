#pragma once

#include "server.hpp"
#include "object.hpp"

#include <utility>
#include <vector>

// World server, does the physics and shit
class world : public server
{
public:
	void start() override;
private:
	// Updates the world, in a different thread
	void check_queue();

	short int tickrate = 30;

	std::vector<object> objects;

	// Pair of Opcode & Server Pointer
	// Calls other servers with specified operation
	// if the world is changed
	std::vector<std::pair<size_t,server*>> callbacks;
};