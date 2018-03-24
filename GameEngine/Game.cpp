#include "Game.h"
#include "Color.h"

GameState::GameState()
{
	makeBoard();
}

void GameState::Draw(GE::Buffer & buffer)
{
	for (auto& g : boardGrid)
	{
		DrawRect<DrawType::Fill>(buffer, g, Color{ 0xffffffff });
	}

	playerHead.draw(buffer);
	for (auto& b : playerBody)
	{
		b.draw(buffer);
	}
}

void GameState::movePlayer()
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

void GameState::processInput()

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

void GameState::Update()
{
	processInput();
	movePlayer();
}

void GameState::ClearInput()
{
	inputs.clear();
}

void GameState::AddInput(KeyInput input)
{
	inputs.push_back(input);
}

void GameState::makeBoard()
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

void GameState::Node::draw(GE::Buffer& buffer)
{
	sprite.x = x * cellSize;
	sprite.y = y * cellSize;
	DrawRect<DrawType::Fill>(buffer, sprite, Color{ 0x90ffffff });
}