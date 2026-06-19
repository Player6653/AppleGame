#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace APPLE_GAME
{
	struct Apple
	{
		Position2D Position;
		sf::Sprite sprite;
		bool isEaten = false;
	};

	struct Game;

	void InitApples(Apple& apple, const Game& game);
	void DrawApple(Apple& apple, sf::RenderWindow& window);
}