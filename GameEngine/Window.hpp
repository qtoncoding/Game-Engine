#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "Buffer.hpp"

namespace GE
{
constexpr int DefaultWindowWidth = 650;
constexpr int DefaultWindowHeight = static_cast<int>(DefaultWindowWidth / (16.0 / 9.0));

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
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

class Window
{
public:
	/// <summary>Create a window</summary>
	/// <param name='instance'>Handle to program Instance</param>
	Window(HINSTANCE instance) : Window(instance, DefaultWindowWidth, DefaultWindowHeight)
	{
	}

	/// <summary>Create a window</summary>
	/// <param name='instance'>Handle to program Instance</param>
	/// <param name='width'>Width of window</param>
	/// <param name='height'>Height of window</param>
	Window(HINSTANCE instance, int width, int height) :
		m_instance(instance),
		m_width(width),
		m_height(height),
		m_handle(makeMainWindow(WndProc)),
		m_dc(GetDC(m_handle))
	{
		RECT clientRect;
		GetClientRect(m_handle, &clientRect);

		m_buffer = Buffer(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}

	/// <summary>Get the frame buffer of this window</summary>
	const Buffer& FrameBuffer() const 
	{ 
		return m_buffer; 
	}

	/// <summary>Get the width of this window</summary>
	int Width() const
	{
		return m_width;
	}

	/// <summary>Get the height of this window</summary>
	int Height() const
	{
		return m_height;
	}

	/// <summary>Get the Device Context of this window</summary>
	HDC DeviceContext() const
	{
		return m_dc;
	}

	/// <summary>Get the Window Handle of this window</summary>
	HWND Handle() const
	{
		return m_handle;
	}

private:
	HINSTANCE m_instance;
	int m_width;
	int m_height;
	HWND m_handle;
	HDC m_dc;
	Buffer m_buffer;

	/// <summary>Create a new Window for displaying content</summary>
	/// <param name='windowProc'>Window Proc function</param>
	HWND makeMainWindow(WNDPROC windowProc)
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

		RECT windowRect{ 0, 0, m_width, m_height };
		auto width = m_width;
		auto height = m_height;
		if (AdjustWindowRectEx(&windowRect, WS_BORDER | WS_CAPTION | WS_SYSMENU, FALSE, 0)) {
			width = windowRect.right - windowRect.left;
			height = windowRect.bottom - windowRect.top;
		}

		auto windowHandle = CreateWindowEx(0,
										   className,
										   windowTitle,
										   WS_BORDER | WS_CAPTION | WS_SYSMENU,
										   CW_USEDEFAULT,
										   CW_USEDEFAULT,
										   width,
										   height,
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
