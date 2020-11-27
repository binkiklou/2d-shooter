#pragma once

class vector2
{
public:
	vector2();
	vector2(float, float);


	vector2 operator + (vector2 const&);
	vector2 operator - (vector2 const&);

	vector2 operator * (vector2 const&);
	vector2 operator / (vector2 const&);

	vector2 operator * (float const&);
	vector2 operator / (float const&);
	

	void print();


	float x;
	float y;
};