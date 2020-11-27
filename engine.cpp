#include "engine.hpp"

#include "logic_opcodes.hpp"
#include "world_opcodes.hpp"

#include <iostream>

#define tickrate 30
#define tickdelay (1000/tickrate)

engine::engine()
{
	/*
	* Order for creating objects should be like this:
	* 1. World
	* 2. Logic
	* 3. Renderer
	*/

	// Creating the world(kinda poetic)
	this->w = new world();

	// Creating the logic server
	this->l = new logic(this->w);
	
	// Creating the renderer
	this->r = new renderer(this->w);
}

// === GET/SETTERS

bool engine::is_running()
{
	return this->running;
}

// === ACTUAL FUNCTIONS ===

void engine::start()
{
	this->w->start();
	this->l->start();
	this->r->start();
	this->running = true;

	// Should be removed
	object* tmp = new object;
	tmp->pos = vector2(5,0);
	tmp->angle = 0;
	tmp->coords.first = vector2(-3,0);
	tmp->coords.second = vector2(3,0);
	message req;
	req.opcode = WORLD_PUSH_OBJECT;
	req.data.push_back((size_t)tmp);
	this->w->push_message(req);
}

void engine::update()
{
	// Check
	if (!this->l->is_ok())
		this->running = false;
	if (!this->w->is_ok())
		this->running = false;
	if (!this->r->is_ok())
		this->running = false;

	// Close in case of emergency
	if (!this->running)
	{
		this->l->stop();
		this->w->stop();
		this->r->stop();

		std::cout << "Engine quitting due to unexpected error" << std::endl;
	}

	sf::sleep(sf::milliseconds(tickdelay));
}

void engine::push_script(script* s)
{
	message m;

	// Opcode
	m.opcode = LOGIC_PUSH_SCRIPT;

	// Params
	m.data.push_back((size_t)s);

	// Last char for sender
	m.data.push_back((size_t)0);

	this->l->push_message(m);
}