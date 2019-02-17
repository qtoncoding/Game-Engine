#include "Buffer.hpp"

#include <stdexcept>
#include <cmath>
#include <cstdlib>

void Impl::drawRectInternal(GE::Buffer & buffer, const Rect & rect, Color fillColor)
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

GE::Buffer::Buffer(int width, int height) :
	m_width(width),
	m_height(height)
{
	info.bmiHeader.biSize = sizeof(info.bmiHeader);
	info.bmiHeader.biWidth = m_width;
	info.bmiHeader.biHeight = -m_height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	data = std::vector<Color>(m_width * m_height);
}

BITMAPINFO * GE::Buffer::BMapInfoPtr() { return &info; }
uint32_t * GE::Buffer::Data() { return reinterpret_cast<uint32_t*>(data.data()); }

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
		/*auto ratio = static_cast<double>(newColor.color.alpha) / 0xff;
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

		this->at(x, y) = colorToDraw;*/
		this->at(x, y) = newColor;
	}
}

void GE::Buffer::DrawRange(int x, int y, int length, Color color)
{
	if (x >= 0 && x < m_width && (x + length < m_width) && y >= 0 && y < m_height)
	{
		auto start = std::next(std::begin(data), (x + (y * m_width)));
		auto end = std::next(start, length);
		std::fill(start, end, color);
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

void GE::Buffer::DrawFrameTime(std::vector<long long>& frameTime)
{
	for (auto i = 0u; i < frameTime.size(); ++i)
	{
		DrawPixel(i, Height() - (static_cast<int>(frameTime[i]) + frameOffset), Color{ 0xffffff00 });
	}
}

void GE::Buffer::DrawTargetFrameTime(int targetFrameTime)
{
	for (auto i = 0; i < m_width; ++i)
	{
		DrawPixel(i, Height() - (targetFrameTime + frameOffset), Color{ 0xff00ff00 });
	}
}

void GE::Buffer::FillFrame()
{
	std::fill(std::begin(data), std::end(data), Color{ 0x00000000 });
}

void GE::Buffer::DrawLine(int x1, int y1, int x2, int y2, Color color)
{
	int startX = (x1 <= x2) ? x1 : x2;
	int endX = (x1 <= x2) ? x2 : x1;
	int startY = (y1 <= y2) ? y1 : y2;
	int endY = (y1 <= y2) ? y2 : y1;

	float deltaY = static_cast<float> (endY - startY);
	float deltaX = static_cast<float> (endX - startX);

	float slope = deltaX / deltaY;
	if (slope > 1) {
		slope = deltaY / deltaX;
		float currY = static_cast<float>(startY);
		for (int x = startX; x <= endX; x++) {
			auto y = static_cast<int>(std::round(currY));
			DrawPixel(x, y, color);
			currY += slope;
		}

	}
	else {
		float currX = static_cast<float>(startX);
		for (int y = startY; y <= endY; y++) {
			auto x = static_cast<int>(std::round(currX));
			DrawPixel(x, y, color);
			currX += slope;
		}
	}
}