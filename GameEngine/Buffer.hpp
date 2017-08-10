#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <algorithm>
#include <vector>

namespace GE
{

struct ColorTuple
{
	unsigned char blue;
	unsigned char red;
	unsigned char green;
	unsigned char alpha;
};

struct Color
{
	union
	{
		uint32_t val;
		ColorTuple color;
	};

	inline Color& operator=(uint32_t value)
	{
		val = value;
		return *this;
	}

	inline Color& operator=(ColorTuple value)
	{
		color = value;
		return *this;
	}
};

class Buffer
{
	BITMAPINFO info;
	std::vector<Color> data;
	int m_width;
	int m_height;

	constexpr static int frameOffset = 50;
public:
	Buffer(int width, int height) :
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

	BITMAPINFO* BMapInfoPtr() { return &info; }
	uint32_t* Data() { return reinterpret_cast<uint32_t*>(data.data()); }

	void drawPixel(int x, int y, uint32_t color)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			data[x + (y * m_width)] = color;
		}
	}

	void DrawFrameTime(std::vector<long long>& frameTime)
	{
		for (auto i = 0u; i < frameTime.size(); ++i)
		{
			drawPixel(i, static_cast<int>(frameTime[i]) + frameOffset, 0x00ffff00);
		}
	}

	void DrawTargetFrameTime(int targetFrameTime)
	{
		for (auto i = 0; i < m_width; ++i)
		{
			drawPixel(i, targetFrameTime + frameOffset, 0x0000ff00);
		}
	}

	void FillFrame()
	{
		std::fill(std::begin(data), std::end(data), 0x00000000);
	}
};

}
	
