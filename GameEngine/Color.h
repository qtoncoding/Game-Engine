#pragma once

#include <variant>

struct Color
{
	constexpr Color(uint32_t c) : val(c) {}

	constexpr Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
		: val((alpha << 24) | (red << 16) | (green << 8) | blue) {}

	constexpr Color(double r, double g, double b)
		: val((0xff << 24) | (static_cast<unsigned char>(255.999 * r) << 16) | (static_cast<unsigned char>(255.999 * g) << 8) | static_cast<unsigned char>(255.999 * b)) {}

	Color() = default;

	Color& operator=(uint32_t c)
	{
		val = c;
		return *this;
	}

	union 
	{
		uint32_t val;
		struct
		{
			unsigned char alpha;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
		} color;
	};
};