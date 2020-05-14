#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <chrono>
#include <thread>
#include <sstream>

#include "Buffer.hpp"

namespace GE
{

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Frame
{
public:
	/// <summary>Create a frame</summary>
	/// <param name='buffer'>Reference to screen buffer</param>
	/// <param name='deviceContext'>Reference to device context</param>
	/// <param name='width'>Frame width</param>
	/// <param name='height'>Frame height</param>
	/// <param name='times'>Reference to collection of frame times, for frame time statistics</param>
	/// <remark>Create a frame object in a new scope before drawing each frame. End of scope will draw buffer to screen.</remark>
	Frame(Buffer* buffer, HDC* deviceContext, int width, int height) :
		m_screenBuffer(buffer),
		m_deviceContext(deviceContext),
		m_width(width),
		m_height(height)
	{
		frameBeginTime = std::chrono::high_resolution_clock::now();
		buffer->ResetChangeStatus();
	}

	~Frame()
	{
		StretchDIBits(*m_deviceContext,
			0, 0, m_width, m_height,
			0, 0, m_width, m_height,
			m_screenBuffer->Data(),
			m_screenBuffer->BMapInfoPtr(),
			DIB_RGB_COLORS,
			SRCCOPY);

		auto frameEndTime = std::chrono::high_resolution_clock::now();

		EndFrame(frameBeginTime, frameEndTime);
	}

	TimePoint frameBeginTime;

private:
	inline
	void EndFrame(TimePoint beginFrame, TimePoint endFrame)
	{
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endFrame - beginFrame);

		if (m_screenBuffer->Changed())
		{
			std::stringstream str;
			str << "Changed\n";
			str << "Drawing took: " << elapsedTime.count() << "ms\n";
			OutputDebugString(str.str().c_str());
		}
	}

	Buffer* m_screenBuffer;
	HDC* m_deviceContext;
	int m_width;
	int m_height;

	constexpr static auto FPS = 30;
	constexpr static auto MillisecondsPerFrame = std::chrono::milliseconds(static_cast<int>(1000.0 / FPS));
};

}
