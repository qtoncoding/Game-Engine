#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <algorithm>
#include <vector>
#include <variant>

#include "Shapes.h"
#include "Color.h"


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

namespace GE
{
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
	Buffer(int width, int height);

	/// <summary>Get pointer to BitMap Info struct of screen buffer</summary>
	BITMAPINFO* BMapInfoPtr();

	/// <summary>Get pointer to screen buffer as c-array of uint32_t</summary>
	uint32_t* Data();


	/// <summary> Get reference to pixel at certain column and row</summary>
	/// <param name='col'>Column of pixel</param> 
	/// <param name='row'>Row of pixel</param>
	/// <exception cref='std::invalid_argument'>Invalid Argument exception thrown when given invalid coordinate</exception>
	Color& at(int col, int row);
	

	/// <summary>Draw pixel at given column and row with color</summary>
	/// <param name='x'>Column of pixel</param> 
	/// <param name='y'>Row of pixel</param>
	/// <param name='color'>Color of pixel</param>
	/// <remark>No-op when given invalid pixel coordinate</remark>
	void DrawPixel(int x, int y, Color newColor);
	

	/// <summary>Return width of buffer</summary>
	int Width() const;
	

	/// <summary>Return height of buffer</summary>
	int Height() const;

	// TODO(Nhung): Implement draw line
	/*void DrawLine(int x1, int y1, int x2, int y2, Color color)
	{

	}*/

	void DrawFrameTime(std::vector<long long>& frameTime);
	void DrawTargetFrameTime(int targetFrameTime);
	void FillFrame();
	void DrawLine(int x1, int y1, int x2, int y2, Color color);
};

};
	
namespace Impl
{
	void drawRectInternal(GE::Buffer& buffer, const Rect& rect, Color fillColor);
}

template <DrawType i>
void DrawRect(GE::Buffer& buffer, const Rect& rect, Color fillColor, [[maybe_unused]] Color outlineColor = Color{ 0x00 })
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

