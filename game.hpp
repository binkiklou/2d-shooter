#pragma once

#include "script.hpp"

class game : public script
{
public:
	game();
	void start() override;
	void update() override;
private:
	vector2* dir;
	int* angle;
};