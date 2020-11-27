#pragma once

#include "server.hpp"
#include "object.hpp"

#include <chrono>
#include <utility>
#include <vector>

// World server, does the physics and shit
class world : public server
{
public:
	void start() override;
private:
	void filter();

	// Updates the world, in a different thread
	void check_queue();

	// Physics functions
	void update_physics(std::queue<message>);
	void move(int,vector2);

	short int tickrate = 30;

	std::chrono::high_resolution_clock::time_point last_tick;

	std::vector<object> objects;

	// Pair of Opcode & Server Pointer
	// Calls other servers with specified operation
	// if the world is changed
	std::vector<std::pair<size_t,server*>> change_cb;

	std::vector<std::pair<size_t, server*>> tick_cb;
};