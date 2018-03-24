#pragma once

#include <variant>

class Color
{
public:
	Color(uint32_t color);
	Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	Color(float red, float green, float blue, float alpha = 1.0);
	Color& operator=(uint32_t color);

	Color() = default;
	Color& operator=(Color const&) = default;
	Color& operator=(Color&&) = default;
	Color(Color const&) = default;
	Color(Color&&) = default;

	union 
	{
		uint32_t val;
		struct
		{
			unsigned char blue;
			unsigned char red;
			unsigned char green;
			unsigned char alpha;
		} color;
	};
};
