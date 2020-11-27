#pragma once

#include "server.hpp"
#include "world.hpp"
#include "script.hpp"

#include <vector>

// Logic server, runs the scripts and stuff like that
class logic : public server
{
public:
	logic();
	logic(world*);

	void start() override;
private:
	void run_logic();

	void push_script(message);

	std::vector<script*> scripts;

	world* w_ptr;
};