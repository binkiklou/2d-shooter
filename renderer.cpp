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

	std::vector<sf::Uint8> buffer((int)(width * height * 4));
	sf::RectangleShape sprite;
	sf::Texture texture;

	texture.create(width,height);
	sprite.setSize(
		sf::Vector2f(
			width,
			height
		)
	);

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
				break;
			}
		}

		this->r_win.clear();
		// ==== RAYCASTING ====
		raycaster.local_objects = this->local_objects;
		raycaster.raycast(100);

		int x = 0;
		for (const ray& r : raycaster.rays)
		{
			if (r.hit)
			{
				// This is a mess
				int offset = (height / 2) - (r.distance * (height / 100));

				//std::cout << offset << std::endl;

				for (int y = (height/2) - (offset-1);y < (height/2) + (offset-1);y++)
				{
					sf::Uint8* ptr = &buffer.at( (y * width + x) * 4);
					ptr[0] = r.r;
					ptr[1] = r.g;
					ptr[2] = r.b;
					ptr[3] = 255;
				}
			}
			x++;
		}

		sprite.setTexture(&texture);
		texture.update(buffer.data());
		this->r_win.draw(sprite);
		// ================================

		this->r_win.display();
	}
}