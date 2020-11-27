#pragma once

#include <vector>

class message
{
public:
	int opcode;

	// Improvements:
	// a - Not use a vector.
	// b - Not use size_t, but instead a datasize
	// that's the same across all platforms, and
	// manage the different sizes in the code.
	std::vector<size_t> data;
};