#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "Buffer.hpp"

namespace GE
{
constexpr int DefaultWindowWidth = 960;
constexpr int DefaultWindowHeight = 640;


LRESULT CALLBACK
WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		BeginPaint(handle, &paint);
		EndPaint(handle, &paint);

		return 0;
	} break;
	case WM_CLOSE:
		[[fallthrough]]
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

class Window
{
public:
	Window(HINSTANCE instance) : 
		m_instance(instance),
		m_width(DefaultWindowWidth),
		m_height(DefaultWindowHeight),
		m_handle(MakeMainWindow(WndProc)),
		m_buffer(Buffer(m_width, m_height)),
		m_dc(GetDC(m_handle))
	{
	}

	const Buffer& FrameBuffer() const 
	{ 
		return m_buffer; 
	}

	int Width() const
	{
		return m_width;
	}

	int Height() const
	{
		return m_height;
	}

	HDC DeviceContext() const
	{
		return m_dc;
	}

	HWND Handle() const
	{
		return m_handle;
	}

private:
	int m_width;
	int m_height;
	HWND m_handle;
	Buffer m_buffer;
	HDC m_dc;
	HINSTANCE m_instance;

	HWND MakeMainWindow(WNDPROC windowProc)
	{
		m_instance = GetModuleHandle(nullptr);

		constexpr auto className = "MainWindow";
		constexpr auto windowTitle = "MainWindow";

		WNDCLASS WindowClass = {};
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = windowProc;
		WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowClass.hInstance = m_instance;
		WindowClass.hIcon = nullptr;
		WindowClass.lpszClassName = className;

		if (!RegisterClass(&WindowClass))
		{
			throw std::runtime_error("Cannot register window class");
		}

		auto windowHandle = CreateWindowEx(0,
										   className,
										   windowTitle,
										   WS_OVERLAPPEDWINDOW,
										   CW_USEDEFAULT,
										   CW_USEDEFAULT,
										   CW_USEDEFAULT,
										   CW_USEDEFAULT,
										   nullptr,
										   nullptr,
										   m_instance,
										   nullptr);

		if (!windowHandle)
		{
			throw std::runtime_error("Cannot create main window");
		}

		ShowWindow(windowHandle, SW_SHOW);
		UpdateWindow(windowHandle);

		return windowHandle;
	}



};

}
