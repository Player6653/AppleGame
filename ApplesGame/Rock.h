#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace APPLE_GAME
{
	struct Rock
	{
		Position2D Position;
		sf::Sprite sprite;
	};

	struct Game;

	void InitRocks(Rock& rock, const Game& game);
	void DrawRock(Rock& rock, sf::RenderWindow& window);
}