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
	/// <summary>Create screen buffer</summary>
	/// <param name='width'>Width of buffer</param>
	/// <param name='height'>Height of buffer</param>
	/// <exception>Can throw exception if out of memory for screen buffer</exception>
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


	/// <summary>Get pointer to BitMap Info struct of screen buffer</summary>
	BITMAPINFO* BMapInfoPtr() { return &info; }

	/// <summary>Get pointer to screen buffer as c-array of uint32_t</summary>
	uint32_t* Data() { return reinterpret_cast<uint32_t*>(data.data()); }

	
	/// <summary> Get reference to pixel at certain column and row</summary>
	/// <param name='col'>Column of pixel</param> 
	/// <param name='row'>Row of pixel</param>
	/// <exception cref='std::invalid_argument'>Invalid Argument exception thrown when given invalid coordinate</exception>
	Color& at(int col, int row)
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

	/// <summary>Draw pixel at given column and row with color</summary>
	/// <param name='x'>Column of pixel</param> 
	/// <param name='y'>Row of pixel</param>
	/// <param name='color'>Color of pixel</param>
	/// <remark>No-op when given invalid pixel coordinate</remark>
	void DrawPixel(int x, int y, Color color)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			this->at(x, y) = color;
		}
	}

	void DrawLine(int x1, int y1, int x2, int y2, Color color)
	{

	}

	void DrawFrameTime(std::vector<long long>& frameTime)
	{
		for (auto i = 0u; i < frameTime.size(); ++i)
		{
			DrawPixel(i, static_cast<int>(frameTime[i]) + frameOffset, Color{ 0x00ffff00 });
		}
	}

	void DrawTargetFrameTime(int targetFrameTime)
	{
		for (auto i = 0; i < m_width; ++i)
		{
			DrawPixel(i, targetFrameTime + frameOffset, Color{ 0x0000ff00 });
		}
	}

	void FillFrame()
	{
		std::fill(std::begin(data), std::end(data), Color{ 0x00000000 });
	}
};

}
	
