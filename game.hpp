#pragma once

#include "script.hpp"
#include "object.hpp"

class game : public script
{
public:
	game();
	void start() override;
	void update() override;
private:
	object* obj;
	vector2* dir;
	int* angle;
};