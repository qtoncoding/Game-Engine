#pragma once

#include <vector>

#include "Shapes.h"
#include "Buffer.hpp"

enum class KeyInput
{
	Up,
	Down,
	Left,
	Right,
	None
};

class GameState
{
private:
	std::vector<KeyInput> inputs;
	std::vector<Rect> boardGrid;
	static constexpr int cellSize = 50;
	static constexpr int rows = 12;
	static constexpr int cols = 19;
	
	struct Node
	{
		int x = 5;
		int y = 5;
		Rect sprite{ x * cellSize, y * cellSize, cellSize, cellSize };
		void draw(GE::Buffer& buffer);
	};
	Node playerHead;
	std::vector<Node> playerBody;

	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};
	Direction currentDirection = Direction::None;

	void makeBoard();
	void movePlayer();
	void processInput();

public:
	GameState();
	void Draw(GE::Buffer& buffer);
	void Update();
	void ClearInput();
	void AddInput(KeyInput input);
};