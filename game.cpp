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
	*this->angle = 0;

	this->obj = new object;
}

void game::update()
{
	bool changed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		message req;
		req.opcode = WORLD_QUIT;
		this->w_ptr->push_message(req);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		dir->x = std::cosf(*this->angle * (3.1415/180));
		dir->y = std::sinf(*this->angle * (3.1415/180));

		//dir->print();

		message req;
		req.opcode = WORLD_PHYSICS_MOVE_OBJECT;
		req.data.push_back(0);
		req.data.push_back((size_t)dir);
		this->w_ptr->push_message(req);

		changed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		dir->x = -std::cosf(*this->angle * (3.1415/180));
		dir->y = -std::sinf(*this->angle * (3.1414/180));

		//dir->print();

		message req;
		req.opcode = WORLD_PHYSICS_MOVE_OBJECT;
		req.data.push_back(0);
		req.data.push_back((size_t)dir);
		this->w_ptr->push_message(req);

		changed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		message req;
		req.opcode = WORLD_PHYSICS_ROTATE_OBJECT;
		req.data.push_back(0);
		*angle -= 3;
		req.data.push_back((size_t)angle);
		this->w_ptr->push_message(req);

		changed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		message req;
		req.opcode = WORLD_PHYSICS_ROTATE_OBJECT;
		req.data.push_back(0);
		*angle += 3;
		req.data.push_back((size_t)angle);
		this->w_ptr->push_message(req);

		changed = true;
	}

	message req;
	req.opcode = WORLD_GET_OBJECT;
	req.data.push_back(0); // wanna get the cam
	req.data.push_back((size_t)obj);
	this->w_ptr->push_message(req);

	if (obj != nullptr && changed == true)
	{
		obj->pos.print();
	}
}