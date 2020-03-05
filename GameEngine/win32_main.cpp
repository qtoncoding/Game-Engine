#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "Window.hpp"
#include "Frame.hpp"
#include "Buffer.hpp"
#include "Game.h"

#include "Engine.hpp"
//
//using namespace GE;
//
//using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
//
//constexpr static auto FPS = 30;
//constexpr static auto MillisecondsPerFrame = std::chrono::milliseconds(static_cast<int>(1000.0 / FPS));
//static bool Running = true;
//
//void ProcessInput(MSG const& msg, GameState& game)
//{
//	OutputDebugString("KeyDown!\n");
//	KeyInput currentInput = KeyInput::None;
//	switch (msg.wParam)
//	{
//	case VK_UP:
//	{
//		currentInput = KeyInput::Up;
//	} break;
//
//	case VK_DOWN:
//	{
//		currentInput = KeyInput::Down;
//	} break;
//
//	case VK_LEFT:
//	{
//		currentInput = KeyInput::Left;
//	} break;
//
//	case VK_RIGHT:
//	{
//		currentInput = KeyInput::Right;
//	} break;
//
//	case VK_ESCAPE:
//	{
//		Running = false;
//	} break;
//
//	default:
//		break;
//	}
//
//	if (currentInput != KeyInput::None)
//	{
//		game.AddInput(currentInput);
//	}
//}
//
//void ProcessWindowMessage(HWND& windowHandle, GameState& game)
//{
//	MSG msg;
//	while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE))
//	{
//		switch (msg.message)
//		{
//		case WM_KEYDOWN:
//		{
//			ProcessInput(msg, game);
//		} break;
//
//		default:
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		} break;
//		}
//	}
//}

int CALLBACK 
WinMain(HINSTANCE,
		HINSTANCE,
		LPSTR,
		int)
{
	//GE::Window window(Instance);

	//auto buffer = window.FrameBuffer();
	//auto deviceContext = window.DeviceContext();
	//auto windowHandle = window.Handle();
	//auto width = buffer.Width();

	//std::vector<long long> frameTime(width);
	//GameState game;

	//auto lastRenderTime = std::chrono::high_resolution_clock::now();

	//while (Running)
	//{
	//	game.ClearInput();

	//	ProcessWindowMessage(windowHandle, game);
	//	
	//	// Do game update
	//	game.Update();
	//
	//	GE::Frame frame(buffer, deviceContext, buffer.Width(), buffer.Height(), frameTime);

	//	// Draw
	//	// Clear bg
	//	buffer.FillFrame();
	//	// Draw game
	//	game.Draw(buffer);
	//	// Draw debug frametime
	//	buffer.DrawTargetFrameTime(33);
	//	buffer.DrawFrameTime(frameTime);
	//}

	return 0;
}