#define NOMINMAX
#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>

#include "Buffer.hpp"
#include <stdexcept>
 

GE::Buffer::Buffer(int width, int height) :
	m_width(width),
	m_height(height)
{
	info.bmiHeader.biSize = sizeof(info.bmiHeader);
	info.bmiHeader.biWidth = m_width;
	info.bmiHeader.biHeight = m_height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	data = std::vector<Color>(m_width * m_height);
}

BITMAPINFO * GE::Buffer::BMapInfoPtr() { return &info; }
uint32_t * GE::Buffer::Data()
{ 
	if (frameData) {
		return reinterpret_cast<uint32_t*>(frameData);
	}

	return reinterpret_cast<uint32_t*>(data.data());
}

Color& GE::Buffer::at(int col, int row)
{
	if (col >= 0 && col < m_width &&
		row >= 0 && row < m_height)
	{
		return data[col + (row * m_width)];
	}
	else
	{
		throw std::invalid_argument("invalid pixel coordinate.");
	}
}

void GE::Buffer::DrawPixel(int x, int y, Color newColor)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		auto ratio = static_cast<double>(newColor.color.alpha) / 0xff;
		auto oldColor = this->at(x, y);

		auto oldR = oldColor.color.red;
		auto oldG = oldColor.color.green;
		auto oldB = oldColor.color.blue;

		auto newR = newColor.color.red;
		auto newG = newColor.color.green;
		auto newB = newColor.color.blue;

		Color colorToDraw;
		colorToDraw.color.red = static_cast<unsigned char>(newR * ratio + oldR * (1 - ratio));
		colorToDraw.color.green = static_cast<unsigned char>(newG * ratio + oldG * (1 - ratio));
		colorToDraw.color.blue = static_cast<unsigned char>(newB * ratio + oldB * (1 - ratio));
		colorToDraw.color.alpha = 0xff;

		this->at(x, y) = colorToDraw;
		changed = true;
	}
}

int GE::Buffer::Width() const
{
	return m_width;
}

int GE::Buffer::Height() const
{
	return m_height;
}

void GE::Buffer::FillFrame()
{
	std::fill(std::begin(data), std::end(data), Color{ 0x00000000 });
}

void GE::Buffer::SetData(const Color* f)
{
	frameData = const_cast<Color*>(f);
}

bool GE::Buffer::Changed() const
{
	return changed;
}

void GE::Buffer::ResetChangeStatus()
{
	changed = false;
}
