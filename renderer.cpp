#include "renderer.hpp"

#include "world_opcodes.hpp"
#include "render_opcodes.hpp"

#include "raycast.hpp"

#include <iostream>

renderer::renderer(world* w)
{
	this->world_ptr = w;

	this->width = 800;
	this->height = 600;
}

void renderer::start()
{
	this->running = true;
	this->m_thread = std::thread(&renderer::render, this);
}

void renderer::update_objects()
{
	std::cout << "" << std::endl;
}

void renderer::check_queue()
{
	if (m_queue.size() > 0)
	{
		this->mtx.lock();

		while (!m_queue.empty())
		{
			message m = m_queue.front();

			switch (m.opcode)
			{
			case RENDER_ASK_OBJECTS:
			{
				message request;
				request.opcode = WORLD_GET_OBJECTS;
				
				request.data.push_back(RENDER_SET_OBJECTS);
				request.data.push_back((size_t)this);

				this->world_ptr->push_message(request);
			}
				break;
			case RENDER_SET_OBJECTS:
				std::vector<object>* ptr = (std::vector<object>*)m.data.at(0);
				this->local_objects = *ptr;
				break;
			}

			this->m_queue.pop();
		}

		this->mtx.unlock();
	}
}

void renderer::render()
{
	std::cout << "Rendering server started" << std::endl;

	r_win.create(sf::VideoMode(800, 600), "Test");

	this->r_win.setFramerateLimit(60);

	raycaster raycaster;

	raycaster.reset(this->width);

	// Then we would have a problem
	if (this->world_ptr != nullptr)
	{
		message m;

		m.opcode = WORLD_CALLBACK_IF_CHANGED;

		m.data.push_back(RENDER_ASK_OBJECTS);

		size_t taddr = (size_t)this;
		m.data.push_back(taddr);

		world_ptr->push_message(m);
	}

	std::vector<sf::Uint8> buffer(width * height * 4);
	sf::RectangleShape sprite;
	sf::Texture texture;

	while (this->r_win.isOpen() && this->running)
	{
		this->check_queue();

		sf::Event event;
		while (this->r_win.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				//this->ok = false;
				this->r_win.close();
				this->running = false;
			}
		}

		this->r_win.clear();
		// ==== RAYCASTING ====

		int x = 0;
		raycaster.local_objects = this->local_objects;
		raycaster.raycast(100);

		for (const ray& r : raycaster.rays)
		{
			if (r.hit)
			{
				sf::RectangleShape l;

				l.setSize(
					sf::Vector2f(1.0f, 100.0f - r.distance)
				);

				l.setPosition(
					sf::Vector2f(
						x,
						this->height/ 2
					)
				);

				this->r_win.draw(l);
			}
			x++;
		}

		// ================================

		this->r_win.display();
	}
}