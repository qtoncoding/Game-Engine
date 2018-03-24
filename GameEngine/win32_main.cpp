#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <mutex>

#include "Window.hpp"
#include "Frame.hpp"
#include "Buffer.hpp"

using namespace GE;

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

constexpr static auto FPS = 30;
constexpr static auto MillisecondsPerFrame = std::chrono::milliseconds(/*static_cast<int>(1000.0 / FPS)*/0);
static bool Running = true;

enum class KeyInput
{
	Up, 
	Down,
	Left,
	Right,
	None
};

struct GameState
{
	std::vector<KeyInput> inputs;
	static const int cellSize = 50;
	static const int rows = 12;
	static const int cols = 19;

	GameState()
	{
		makeBoard();
	}

	std::vector<Rect> boardGrid;
	void makeBoard()
	{
		for (auto i = 0; i < rows; ++i)
		{
			boardGrid.emplace_back(Rect{ 0, i*cellSize, 1000, 2 });
		}

		for (auto i = 0; i < cols; ++i)
		{
			boardGrid.emplace_back(Rect{ i*cellSize, 0, 2, 1000 });
		}
	}

	struct Node
	{
		int x = 5;
		int y = 5;
		Rect sprite{ x * cellSize, y * cellSize, cellSize, cellSize };
		void draw(Buffer& buffer)
		{
			sprite.x = x * cellSize;
			sprite.y = y * cellSize;
			DrawRect<DrawType::Fill>(buffer, sprite, Color{ 0x90ffffff });
		}
	};

	Node playerHead;
	std::vector<Node> playerBody;

	void draw(Buffer& buffer)
	{
		// StopWatch watch;
		// Stopwatch.Out (message)
		// Message + time + ms


		auto beforeGrid = std::chrono::high_resolution_clock::now();
		for (auto& g : boardGrid)
		{
			DrawRect<DrawType::Fill>(buffer, g, Color{ 0xffffffff });
		}
		auto gridTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beforeGrid);

		std::stringstream str;
		str << "Grid Draw time: " << gridTime.count() << "ms\n";
		OutputDebugString(str.str().c_str());

		auto beforePlayer = std::chrono::high_resolution_clock::now();
		playerHead.draw(buffer);
		for (auto& b : playerBody)
		{
			b.draw(buffer);
		}
		auto playerTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beforePlayer);

		str.clear();
		str << "Player Draw time: " << playerTime.count() << "ms\n";
		OutputDebugString(str.str().c_str());

	}

	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};

	Direction currentDirection = Direction::None;

	void movePlayer()
	{
		if (currentDirection != Direction::None)
		{
			playerBody.push_back(Node(playerHead));
			std::rotate(std::begin(playerBody), std::prev(std::end(playerBody)), std::end(playerBody));
		}

		switch (currentDirection)
		{
		case Direction::Up:
		{
			playerHead.y += 1;
		} break;

		case Direction::Down:
		{
			playerHead.y -= 1;
		} break;

		case Direction::Left:
		{
			playerHead.x -= 1;
		} break;

		case Direction::Right:
		{
			playerHead.x += 1;
		} break;

		default:
			break;
		}
		currentDirection = Direction::None;
	}

	void processInput()
	{
		if (!inputs.empty())
		{
			for (auto& input : inputs)
			{
				switch (input)
				{
				case KeyInput::Up:
				{
					currentDirection = Direction::Up;
				} break;

				case KeyInput::Down:
				{
					currentDirection = Direction::Down;
				} break;

				case KeyInput::Left:
				{
					currentDirection = Direction::Left;
				} break;

				case KeyInput::Right:
				{
					currentDirection = Direction::Right;
				} break;

				default:
					break;
				}
			}
		}
	}

	void update()
	{
		processInput();
		movePlayer();
	}
};


int CALLBACK 
WinMain(HINSTANCE Instance,
		HINSTANCE,
		LPSTR,
		int)
{
	GE::Window window(Instance);
	auto deviceContext = window.DeviceContext();
	auto buffer = window.FrameBuffer(); 
	auto width = buffer.Width();
	std::vector<long long> frameTime(width);

	while (true) {
		GE::Frame frame(buffer, deviceContext, buffer.Width(), buffer.Height(), frameTime);
		buffer.DrawImage("flappyModel.png");
	}
	

	//GameState game;
	//game.makeBoard();

	//auto lastRenderTime = std::chrono::high_resolution_clock::now();

	//while (Running)
	//{
	//	game.inputs.clear();
	//	MSG msg;
	//	while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE))
	//	{
	//		switch (msg.message)
	//		{
	//			case WM_KEYDOWN:
	//			{
	//				OutputDebugString("KeyDown!\n");
	//				KeyInput currentInput = KeyInput::None;
	//				switch (msg.wParam)
	//				{
	//					case VK_UP:
	//					{
	//						currentInput = KeyInput::Up;
	//					} break;

	//					case VK_DOWN:
	//					{
	//						currentInput = KeyInput::Down;
	//					} break;
	//					
	//					case VK_LEFT:
	//					{
	//						currentInput = KeyInput::Left;
	//					} break;
	//					
	//					case VK_RIGHT:
	//					{
	//						currentInput = KeyInput::Right;
	//					} break;

	//					case VK_ESCAPE:
	//					{
	//						Running = false;
	//					} break;
	//					default:
	//						break;
	//				}

	//				if (currentInput != KeyInput::None)
	//				{
	//					game.inputs.push_back(currentInput);
	//				}
	//			} break;

	//			default:
	//			{
	//				TranslateMessage(&msg);
	//				DispatchMessage(&msg);
	//			} break;
	//		}
	//	}

	//	// Do game update
	//	game.update();

	//	auto currentTime = std::chrono::high_resolution_clock::now();
	//	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRenderTime);
	//	if (elapsedTime >= MillisecondsPerFrame)
	//	{
	//		std::stringstream str;
	//		str << "Elapsed time: " << elapsedTime.count() << "ms Target time: " << MillisecondsPerFrame.count() << "ms\n";
	//		OutputDebugString(str.str().c_str());

	//		GE::Frame frame(buffer, deviceContext, buffer.Width(), buffer.Height(), frameTime);
	//		// Draw

	//		// Clear bg
	//		auto beforeClearFrame = std::chrono::high_resolution_clock::now();
	//		buffer.FillFrame();
	//		auto clearFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beforeClearFrame);
	//		
	//		str.clear();
	//		str << "Clear frame time: " << clearFrameTime.count() << "ms\n";
	//		OutputDebugString(str.str().c_str());
	//		
	//		// Draw game
	//		auto beforeGameDraw = std::chrono::high_resolution_clock::now();
	//		game.draw(buffer);
	//		auto gameDrawTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beforeGameDraw);

	//		str.clear();
	//		str << "Game draw time: " << gameDrawTime.count() << "ms\n";
	//		OutputDebugString(str.str().c_str());

	//		// Draw debug frametime
	//		auto beforeDebugDraw = std::chrono::high_resolution_clock::now();
	//		buffer.DrawTargetFrameTime(33);
	//		buffer.DrawFrameTime(frameTime);
	//		auto debugDrawTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beforeGameDraw);

	//		str.clear();
	//		str << "debug draw time: " << debugDrawTime.count() << "ms\n";
	//		OutputDebugString(str.str().c_str());


	//		lastRenderTime = std::chrono::high_resolution_clock::now();

	//		str.clear();
	//		str << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
	//		OutputDebugString(str.str().c_str());

	//	}
	//}

	return 0;
}