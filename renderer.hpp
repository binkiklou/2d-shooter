#pragma once

#include "server.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>

#include <thread>

// Rendering server.. renders stuff
class renderer : public server
{
public:
	renderer(world*);
	
	void start() override;
private:
	void update_objects();
	void render();
	void check_queue();

	sf::RenderWindow r_win; // Render Window
	std::vector<object> local_objects;
	world* world_ptr = nullptr;

	int width;
	int height;
};