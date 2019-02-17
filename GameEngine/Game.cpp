#include "Game.h"
#include "Buffer.hpp"
#include <cmath>

constexpr int GridSize = 35;

char GameState::getCell(int x, int y) const
{
	return MapData[y * Width + x];
}

void GameState::drawMap(GE::Buffer& buffer)
{
	for (auto pixelY = 0; pixelY < GridSize * Height; ++pixelY)
	{
		for (auto gridX = 0; gridX < Width; ++gridX)
		{
			auto gridPositionY = pixelY / GridSize;

			if (gridPositionY < Height)
			{
				auto cell = getCell(gridX, gridPositionY);
				if (cell == '0')
				{
					buffer.DrawRange(gridX * GridSize, pixelY, GridSize, Color{ 128, 128, 128 });
				}
				else if (cell == '1')
				{
					buffer.DrawRange(gridX * GridSize, pixelY, GridSize, Color{ 0, 128, 128 });
				}
				else if (cell == '2')
				{
					buffer.DrawRange(gridX * GridSize, pixelY, GridSize, Color{ 128, 128, 0 });
				}
			}
		}
	}
}

void GameState::drawPlayer(GE::Buffer& buffer)
{
	Rect playerRect{ static_cast<int>(playerX * GridSize), static_cast<int>(playerY * GridSize), 5, 5 };
	DrawRect<DrawType::Fill>(buffer, playerRect, Color{ 0, 255, 0 });
}

char hit(int width, int x, int y, char const* map)
{
	return map[x + y * width];
}

constexpr double PI = 3.1415926535897932384626433;

void GameState::drawFOV(GE::Buffer& buffer)
{
	constexpr double fov = PI / 3.0;
	for (auto col = 0; col < (GridSize * Width); ++col)
	{
		auto angle = playerA - (fov / 2.) + ((fov * col) / (GridSize * Width));

		for (auto step = 0.0; step < 20; step += .01)
		{
			auto cx = playerX + step * std::cos(angle);
			auto cy = playerY + step * std::sin(angle);
			auto hitType = hit(Width, static_cast<int>(cx), static_cast<int>(cy), MapData);
			if (hitType != ' ')
			{
				auto height = buffer.Height() / (step * cos(angle - playerA));
				auto yOffset = static_cast<int>((buffer.Height() - height) / 2.);
				Rect column{ col + (buffer.Width() / 2), yOffset, 1, static_cast<int>(std::round(height))};
				auto wallColor = Color{ 128, 128, 128};
				switch (hitType)
				{
					case '1':
					{
						wallColor = Color{ 0, 128, 128 };
					} break;

					case '2':
					{
						wallColor = Color{ 128, 128, 0 };
					} break;

					default:
						break;
				}
				DrawRect<DrawType::Fill>(buffer, column, wallColor);
				break;
			}

			auto x = static_cast<int>(cx * GridSize);
			auto y = static_cast<int>(cy * GridSize);
			buffer.DrawPixel(x, y, Color{ 255, 255, 255 });
		}
	}
}

void GameState::Draw(GE::Buffer & buffer)
{
	drawMap(buffer);
	drawPlayer(buffer);
	drawFOV(buffer);
}

void GameState::Update()
{
	playerA -= 0.0025;
}