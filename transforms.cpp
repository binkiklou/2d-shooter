#include "transforms.hpp"

#include <iostream>

// === CONSTRUCTORS ===

vector2::vector2()
{
	this->x = 0;
	this->y = 0;
}

vector2::vector2(float a, float b)
{
	this->x = a;
	this->y = b;
}
// === OPERATORS ===

vector2 vector2::operator + (vector2 const& obj)
{
	vector2 result = *this;
	result.x += obj.x;
	result.y += obj.y;
	return result;
}

vector2 vector2::operator-(vector2 const& obj)
{
	vector2 result = *this;
	result.x -= obj.x;
	result.y -= obj.y;
	return result;
}

vector2 vector2::operator*(vector2 const& obj)
{
	vector2 result = *this;
	result.x *= obj.x;
	result.y *= obj.y;
	return result;
}

vector2 vector2::operator/(vector2 const& obj)
{
	vector2 result = *this;
	result.x /= obj.x;
	result.y /= obj.y;
	return result;
}

vector2 vector2::operator*(float const& obj)
{
	vector2 result = *this;
	result.x *= obj;
	result.y *= obj;
	return result;
}

vector2 vector2::operator/(float const& obj)
{
	vector2 result = *this;
	result.x /= obj;
	result.y /= obj;
	return result;
}

// === FUN FUNCTIONS ===

void vector2::print()
{
	std::cout << "[" << this->x << "," << this->y << "]" << std::endl;
}