#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <array>

#include "Window.hpp"
#include "Frame.hpp"
#include "Buffer.hpp"
#include "Game.h"

using namespace GE;

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

constexpr static auto FPS = 30;
constexpr static auto MillisecondsPerFrame = std::chrono::milliseconds(static_cast<int>(1000.0 / FPS));
static bool Running = true;

enum class KeyInput
{
	Up,
	Down,
	Left,
	Right,
	None
};

void ProcessInput(MSG const& msg, [[maybe_unused]]GameState& game)
{
	OutputDebugString("KeyDown!\n");
	KeyInput currentInput = KeyInput::None;
	switch (msg.wParam)
	{
	case VK_UP:
	{
		currentInput = KeyInput::Up;
	} break;

	case VK_DOWN:
	{
		currentInput = KeyInput::Down;
	} break;

	case VK_LEFT:
	{
		currentInput = KeyInput::Left;
	} break;

	case VK_RIGHT:
	{
		currentInput = KeyInput::Right;
	} break;

	case VK_ESCAPE:
	{
		Running = false;
	} break;

	default:
		break;
	}
}

void ProcessWindowMessage(HWND& windowHandle, GameState& game)
{
	MSG msg;
	while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
		{
			ProcessInput(msg, game);
		} break;

		default:
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} break;
		}
	}
}

int CALLBACK 
WinMain(HINSTANCE Instance,
		HINSTANCE,
		LPSTR,
		int)
{
	GE::Window window(Instance);

	auto buffer = window.FrameBuffer();
	auto deviceContext = window.DeviceContext();
	auto windowHandle = window.Handle();
	auto width = buffer.Width();

	std::vector<long long> frameTime(width);

	constexpr size_t mapWidth = 16; // map width
	constexpr size_t mapHeight = 16; // map height
	auto mapData = 
		"0000222222220000"\
		"1              0"\
		"1      11111   0"\
		"1     0        0"\
		"0     0  1110000"\
		"0     3        0"\
		"0   10000      0"\
		"0   0   11100  0"\
		"0   0   0      0"\
		"0   0   1  00000"\
		"0       1      0"\
		"2       1      0"\
		"0       0      0"\
		"0 0000000      0"\
		"0              0"\
		"0002222222200000"; // our game map

	GameState game(mapWidth, mapHeight, mapData);
	auto totalDistance = 12.0;
	constexpr double startY = 3.456;
	auto distanceLeftY = totalDistance - startY;
	game.PlayerX(1.845);
	game.PlayerY(startY);
	game.PlayerA(1.523);

	auto lastRenderTime = std::chrono::high_resolution_clock::now();
	auto lastUpdateTime = lastRenderTime;
	int direction = -1;

	while (Running)
	{
		ProcessWindowMessage(windowHandle, game);
		
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count();
		if (elapsedTimeMs > 15)
		{
			game.PlayerA(game.PlayerA() - (0.0025 * (elapsedTimeMs / 15.)));
			if (distanceLeftY <= 0 || distanceLeftY > totalDistance - startY)
			{
				direction *= -1;
			}
			distanceLeftY += direction * (0.01 * (elapsedTimeMs / 15.));
			game.PlayerY(totalDistance - distanceLeftY);

			lastUpdateTime = currentTime;
		}
		// Do game update
		game.Update();
	
		GE::Frame frame(buffer, deviceContext, buffer.Width(), buffer.Height(), frameTime);

		// Draw
		// Clear bg
		buffer.FillFrame();
		// Draw game
		game.Draw(buffer);
		// Draw debug frametime
		buffer.DrawTargetFrameTime(33);
		buffer.DrawFrameTime(frameTime);
	}

	return 0;
}