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

Color::Color(double red, double green, double blue, double alpha)
{
	color.red =   static_cast<unsigned char>(static_cast<int>(255.99 * red));
	color.green = static_cast<unsigned char>(static_cast<int>(255.99 * green));
	color.blue =  static_cast<unsigned char>(static_cast<int>(255.99 * blue));
	color.alpha = static_cast<unsigned char>(static_cast<int>(255.99 * alpha));
}

Color& Color::operator=(uint32_t c)
{
	val = c;
	return *this;
}
