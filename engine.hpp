#pragma once

#include "world.hpp"
#include "logic.hpp"
#include "renderer.hpp"

class engine
{
public:
	engine();

	// Get/Set
	bool is_running();

	// Functions that do stuff
	void start();
	void update();

	// High-Level Wrapping
	void push_script(script*);
private:
	bool running = false;

	world* w;
	logic* l;
	renderer* r;
};