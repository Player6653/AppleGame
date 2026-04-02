#include "Math.h"
#include <cstdlib>
#include <SFML/Graphics.hpp>

namespace APPLE_GAME
{
	Position2D GetRandomPositionInScreen(float screen_width, float screen_height)
	{
		Position2D result;
		result.x = rand() / (float)RAND_MAX * screen_width;
		result.y = rand() / (float)RAND_MAX * screen_height;
		return result;
	}

	bool IsRectanglesCoolide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size)
	{
		float dx = (float)fabs(rect1Position.x - rect2Position.x);
		float dy = (float)fabs(rect1Position.y - rect2Position.y);
		return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
			dy <= (rect1Size.y + rect2Size.y) / 2.f);
	}

	bool IsCirclesCollide(Position2D circles1Position, float circles1Radius, Position2D circles2Position, float circles2Radius)
	{
		float squareDistance = (circles1Position.x - circles2Position.x) * (circles1Position.x - circles2Position.x) +
			(circles1Position.y - circles2Position.y) * (circles1Position.y - circles2Position.y);
		float squareRadiusSum = (circles1Radius + circles2Radius) * (circles1Radius + circles2Radius);
		return squareDistance <= squareRadiusSum;
	}

	void SetSpriteSize(sf::Sprite& sprite, float desiredWidht, float desiredHeight)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = { desiredWidht / spriteRect.width, desiredHeight / spriteRect.height };
		sprite.setScale(scale);
	}

	void SetSpriteRelativeOrigine(sf::Sprite& sprite, float originX, float originY)
	{ 
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
	}
}