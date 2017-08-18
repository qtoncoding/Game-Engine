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

struct Rect
{
	int x;
	int y;
	int width;
	int height;
};

enum class DrawType : uint8_t
{
	Fill = 0b1,
	Outline = 0b10,
	FillAndOutline = 0b11
};

constexpr bool operator& (const DrawType& a, const DrawType& b)
{
	return 	(static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

class Buffer
{
	BITMAPINFO info;
	std::vector<Color> data;
	int m_width;
	int m_height;

	constexpr static int frameOffset = 80;

public:
	Buffer() {}
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
	void DrawPixel(int x, int y, Color newColor)
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
		}
	}

	/// <summary>Return width of buffer</summary>
	int Width() const
	{
		return m_width;
	}

	/// <summary>Return height of buffer</summary>
	int Height() const
	{
		return m_height;
	}

	// TODO(Nhung): Implement draw line
	/*void DrawLine(int x1, int y1, int x2, int y2, Color color)
	{

	}*/

	void DrawFrameTime(std::vector<long long>& frameTime)
	{
		for (auto i = 0u; i < frameTime.size(); ++i)
		{
			DrawPixel(i, static_cast<int>(frameTime[i]) + frameOffset, Color{ 0xffffff00 });
		}
	}

	void DrawTargetFrameTime(int targetFrameTime)
	{
		for (auto i = 0; i < m_width; ++i)
		{
			DrawPixel(i, targetFrameTime + frameOffset, Color{ 0xff00ff00 });
		}
	}

	void FillFrame()
	{
		std::fill(std::begin(data), std::end(data), Color{ 0x00000000 });
	}
};

namespace Impl
{
	void drawRectInternal(GE::Buffer& buffer, const Rect& rect, Color fillColor)
	{
		int startX = rect.x >= 0 ? rect.x : 0;
		int startY = rect.y >= 0 ? rect.y : 0;
		int endX = startX + rect.width < buffer.Width() ? startX + rect.width : buffer.Width();
		int endY = startY + rect.height < buffer.Height() ? startY + rect.height : buffer.Height();

		for (auto x = startX; x < endX; ++x)
		{
			for (auto y = startY; y < endY; ++y)
			{
				buffer.DrawPixel(x, y, fillColor);
			}
		}
	}
}

template <DrawType i>
void DrawRect(Buffer& buffer, const Rect& rect, Color fillColor, [[maybe_unused]] Color outlineColor = Color{ 0x00 })
{
	Rect innerRect = rect;
	Color innerColor = fillColor;
	if constexpr (i & DrawType::Outline)
	{
		Impl::drawRectInternal(rect, outlineColor);
		constexpr int strokeWidth = 4;
		innerRect.x += strokeWidth;
		innerRect.y += strokeWidth;
		innerRect.width -= strokeWidth;
		innerRect.height -= strokeWidth;
		if constexpr (!(i & DrawType::Fill))
		{
			innerColor = Color{ 0x0 };
		}
	}

	Impl::drawRectInternal(buffer, innerRect, innerColor);
}

}
	
