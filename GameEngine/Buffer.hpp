#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <algorithm>
#include <vector>
#include <variant>
#include <random>
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

	Color* frameData = nullptr;

	bool changed = false;
public:
	Buffer() {}

	Buffer(int width, int height);

	BITMAPINFO* BMapInfoPtr();

	uint32_t* Data();

	Color& at(int col, int row);
	
	void DrawPixel(int x, int y, Color newColor);
	
	int Width() const;
	
	int Height() const;

	void FillFrame();
	void SetData(const Color* f);
	bool Changed() const;
	void ResetChangeStatus();

};

};