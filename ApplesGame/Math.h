#pragma once

namespace sf
{
	class Sprite;
}

namespace APPLE_GAME
{
	struct Vector2D
	{
		float x = 0;
		float y = 0;
	};

	typedef Vector2D Position2D;

	Position2D GetRandomPositionInScreen(float screen_width, float screen_height, float marginX = 0.f, float marginY = 0.f);

	bool IsRectanglesCoolide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size);

	bool IsCirclesCollide(Position2D circles1Position, float circles1Radius, Position2D circles2Position, float circles2Radius);

	void SetSpriteSize(sf::Sprite& sprite, float desiredWidht, float desiredHeight);

	void SetSpriteRelativeOrigine(sf::Sprite& sprite, float originX, float originY);
}