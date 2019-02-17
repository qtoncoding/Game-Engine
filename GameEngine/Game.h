#pragma once

#include "Shapes.h"
#include "Buffer.hpp"


class GameState
{
private:
	int Width;
	int Height;
	
	double playerX;
	double playerY;
	double playerA;

	char const* MapData;

	char getCell(int x, int y) const;

	void drawMap(GE::Buffer& buffer);

	void drawPlayer(GE::Buffer& buffer);
	void rayCast(GE::Buffer& buffer, double angle, bool draw = true);
	void drawFOV(GE::Buffer& buffer);
public:
	GameState(int width, int height, char const* map) :
		Width(width), Height(height), MapData(map) 
	{}

	void PlayerX(double val)
	{
		playerX = val;
	}

	void PlayerY(double val)
	{
		playerY = val;
	}

	void PlayerA(double val)
	{
		playerA = val;
	}

	double PlayerA() const
	{
		return playerA;
	}

	double PlayerX() const
	{
		return playerX;
	}

	double PlayerY() const
	{
		return playerY;
	}

	void Draw(GE::Buffer& buffer);
	void Update();
};