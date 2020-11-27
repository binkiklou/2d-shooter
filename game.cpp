#include "game.hpp"

#include "world_opcodes.hpp"

#include <iostream>

#include <SFML/Window.hpp>

game::game()
{
	// Does nothing
}

void game::start()
{
	this->dir = new vector2;
	this->angle = new int;
}

void game::update()
{
	std::cout << "oii mate I be updating" << std::endl;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		dir->x = 1;
		dir->y = 1;

		message req;
		req.opcode = WORLD_PHYSICS_MOVE_OBJECT;
		req.data.push_back(0);
		req.data.push_back((size_t)dir);
		this->w_ptr->push_message(req);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		dir->x = -1;
		dir->y = -1;

		message req;
		req.opcode = WORLD_PHYSICS_MOVE_OBJECT;
		req.data.push_back(0);
		req.data.push_back((size_t)dir);
		this->w_ptr->push_message(req);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		message req;
		req.opcode = WORLD_PHYSICS_ROTATE_OBJECT;
		req.data.push_back(0);
		*angle -= 1;
		*angle %= 360;
		req.data.push_back((size_t)angle);
		this->w_ptr->push_message(req);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		message req;
		req.opcode = WORLD_PHYSICS_ROTATE_OBJECT;
		req.data.push_back(0);
		*angle += 1;
		*angle %= 360;
		req.data.push_back((size_t)angle);
		this->w_ptr->push_message(req);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		message req;
		req.opcode = WORLD_QUIT;
		this->w_ptr->push_message(req);
	}
}