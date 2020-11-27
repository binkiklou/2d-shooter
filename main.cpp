#include "engine.hpp"
#include "game.hpp"

#include <iostream>
#include <string>

int main()
{
	engine e;

	e.start();

	game* g = new game();

	e.push_script(g);

	while (e.is_running())
	{
		e.update();
	}
	return 0;
}