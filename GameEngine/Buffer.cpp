//#include "Buffer.hpp"
//#include <stdexcept>
//#include <cmath>
//using namespace GE;
//
//void Impl::drawRectInternal(GE::Buffer & buffer, const Rect & rect, GE::Pixel fillPixel)
//{
//	int startX = rect.x >= 0 ? rect.x : 0;
//	int startY = rect.y >= 0 ? rect.y : 0;
//	int endX = startX + rect.width < buffer.Width() ? startX + rect.width : buffer.Width();
//	int endY = startY + rect.height < buffer.Height() ? startY + rect.height : buffer.Height();
//
//	for (auto x = startX; x < endX; ++x)
//	{
//		for (auto y = startY; y < endY; ++y)
//		{
//			buffer.DrawPixel(x, y, fillPixel);
//		}
//	}
//}
//
//GE::Buffer::Buffer(int width, int height) :
//	m_width(width),
//	m_height(height)
//{
//	info.bmiHeader.biSize = sizeof(info.bmiHeader);
//	info.bmiHeader.biWidth = m_width;
//	info.bmiHeader.biHeight = m_height;
//	info.bmiHeader.biPlanes = 1;
//	info.bmiHeader.biBitCount = 32;
//	info.bmiHeader.biCompression = BI_RGB;
//
//	data = std::vector<Pixel>(m_width * m_height);
//}
//
//BITMAPINFO * GE::Buffer::BMapInfoPtr() { return &info; }
//uint32_t * GE::Buffer::Data() { return reinterpret_cast<uint32_t*>(data.data()); }
//
//Pixel& GE::Buffer::at(int col, int row)
//{
//	if (col >= 0 && col < m_width &&
//		row >= 0 && row < m_height)
//	{
//		return data[col + (row * m_width)];
//	}
//	else
//	{
//		throw std::invalid_argument("invalid pixel coordinate.");
//	}
//}
//
//void GE::Buffer::DrawPixel(int x, int y, Pixel newPixel)
//{
//	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
//	{
//		auto ratio = static_cast<double>(newPixel.alpha) / 0xff;
//		auto oldPixel = this->at(x, y);
//
//		auto oldR = oldPixel.red;
//		auto oldG = oldPixel.green;
//		auto oldB = oldPixel.blue;
//
//		auto newR = newPixel.red;
//		auto newG = newPixel.green;
//		auto newB = newPixel.blue;
//
//		Pixel PixelToDraw;
//		PixelToDraw.red = static_cast<unsigned char>(newR * ratio + oldR * (1 - ratio));
//		PixelToDraw.green = static_cast<unsigned char>(newG * ratio + oldG * (1 - ratio));
//		PixelToDraw.blue = static_cast<unsigned char>(newB * ratio + oldB * (1 - ratio));
//		PixelToDraw.alpha = 0xff;
//
//		this->at(x, y) = PixelToDraw;
//	}
//}
//
//int GE::Buffer::Width() const
//{
//	return m_width;
//}
//
//int GE::Buffer::Height() const
//{
//	return m_height;
//}
//
//void GE::Buffer::DrawFrameTime(std::vector<long long>& frameTime)
//{
//	for (auto i = 0u; i < frameTime.size(); ++i)
//	{
//		DrawPixel(i, static_cast<int>(frameTime[i]) + frameOffset, Pixel{ 0xffffff00 });
//	}
//}
//
//void GE::Buffer::DrawTargetFrameTime(int targetFrameTime)
//{
//	for (auto i = 0; i < m_width; ++i)
//	{
//		DrawPixel(i, targetFrameTime + frameOffset, Pixel{ 0xff00ff00 });
//	}
//}
//
//void GE::Buffer::FillFrame()
//{
//	std::fill(std::begin(data), std::end(data), Pixel{ 0x00000000 });
//}
//
//void GE::Buffer::DrawLine(int x1, int y1, int x2, int y2, Pixel Pixel)
//{
//	int startX = (x1 <= x2) ? x1 : x2;
//	int endX = (x1 <= x2) ? x2 : x1;
//	int startY = (y1 <= y2) ? y1 : y2;
//	int endY = (y1 <= y2) ? y2 : y1;
//
//	float deltaY = static_cast<float> (endY - startY);
//	float deltaX = static_cast<float> (endX - startX);
//
//	float slope = deltaX / deltaY;
//	if (slope > 1) {
//		slope = deltaY / deltaX;
//		float currY = static_cast<float>(startY);
//		for (int x = startX; x <= endX; x++) {
//			auto y = static_cast<int>(std::round(currY));
//			DrawPixel(x, y, Pixel);
//			currY += slope;
//		}
//
//	}
//	else {
//		float currX = static_cast<float>(startX);
//		for (int y = startY; y <= endY; y++) {
//			auto x = static_cast<int>(std::round(currX));
//			DrawPixel(x, y, Pixel);
//			currX += slope;
//		}
//	}
//}