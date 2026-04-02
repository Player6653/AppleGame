#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace APPLE_GAME
{
	enum class PlayerDirection
	{
		Zero = 0,
		Right,
		Up,
		Left,
		Down
	};

	struct Player
	{
		Position2D Position;
		float Speed = INITIAL_SPEED;
		PlayerDirection Direction = PlayerDirection::Zero;
		sf::Sprite sprite;
	};

	struct Game;

	void InitPlayer(Player& player, const Game& game);
	void DrawPlayer(Player& player, sf::RenderWindow& window);
	void UpdatePlayer(Player& player, float deltaTime);
}