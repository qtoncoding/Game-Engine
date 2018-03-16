#include "Color.h"

Color::Color(uint32_t c)
{
	val = c;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	color.red = r;
	color.green = g;
	color.blue = b;
	color.alpha = a;
}

Color& Color::operator=(uint32_t c)
{
	val = c;
	return *this;
}
