#include "renderer.hpp"

#include "world_opcodes.hpp"
#include "render_opcodes.hpp"

#include <iostream>
#include <chrono>

#define FOV 80

renderer::renderer(world* w)
{
	this->world_ptr = w;
	this->changed = true;
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
		auto t1 = std::chrono::high_resolution_clock::now();
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

				this->changed = true;
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
		auto t2 = std::chrono::high_resolution_clock::now();

		auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		if (d > 60)
		{
			std::cout << "Checking queue slow: " << d << std::endl;
		}
		//std::cout << "Checking queue took" << d << std::endl;
	}
}

void renderer::render()
{
	std::cout << "Rendering server started" << std::endl;

	r_win.create(sf::VideoMode(800, 600), "Test");

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

	std::cout << buffer.size() << std::endl;

	sf::RectangleShape sprite;
	sf::Texture texture;

	texture.create(width,height);
	sprite.setSize(
		sf::Vector2f(
			width,
			height
		)
	);


	// Wait for camera to be added
	while (local_objects.size() == 0) {
		check_queue();
	}

	while (this->r_win.isOpen() && this->running)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
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

		object cam = local_objects.at(0);

		// ==== RAYCASTING ====

		// Deviation in angle
		float da = (float)FOV / this->width;
		float angle = cam.angle - (da*(FOV/2));

		for (int x = -(width/2); x < (width/2); x++)
		{
			angle = (cam.angle - (da * (FOV / 2))) + (x*da);

			vector2 dir = vector2(std::cosf(angle * (3.14f/180.f)),std::sinf(angle * (3.14f / 180.f)) );
		//	dir.print();

			bool hit = false;

			for (int i = 0; i < 100; i++)
			{
				if (hit)
					break;

				vector2 p = cam.pos + (dir * i * 2);
				//p.x = p.x * i;
				//p.y = p.y * i;

				for (object obj : this->local_objects)
				{
					if (hit)
						break;

					for (vector2 tile : obj.tiles)
					{
						if (
							p.x >= obj.pos.x + (tile.x - 16) &&
							p.y >= obj.pos.y + (tile.y - 16) &&
							p.x <= obj.pos.x + (tile.x + 16) &&
							p.y <= obj.pos.y + (tile.y + 16))
						{

							//p.print();

							double dist = std::sqrt(
								std::powf(std::abs(cam.pos.x - (p.x+16)), 2)
								+
								std::powf(std::abs(cam.pos.y - (p.y+16)), 2)
							);

							//dist = std::cosf((float)FOV/2) * dist;

							double h = 16 / dist * (width / 2 / std::tan((FOV / 2) * (3.1415 / 180)));

							if (h >= height)
							{
								h = height - 1;
							}

							for (int y = (height/2) - (h/2); y < (height/2) + (h/2); y++)
							{
								sf::Uint8* ptr = &buffer.at((y * (width) + (x + (width/2))) * 4);
								int col = 255;
								ptr[0] = col; //- (dist / 100);
								ptr[1] = col; //- (dist / 100);
								ptr[2] = col; //- (dist / 100);
								ptr[3] = col; //- (dist / 100);
							}

							hit = true;

							if(hit)
								break;
						}
					}
				}
			}
		}

		// ================================

		sprite.setTexture(&texture);
		texture.update(buffer.data());
		this->r_win.draw(sprite);
		std::memset(buffer.data(), 0, buffer.size());
		this->r_win.display();

		auto t2 = std::chrono::high_resolution_clock::now();
		auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		if (d > 60)
		{
			std::cout << "Renderer slow: "<<d<<std::endl;
		}
	}
}