#include "Apple.h"
#include "Game.h"

namespace APPLE_GAME
{
	void InitApples(Apple& apple, const Game& game)
	{
		apple.Position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		// Инициализация спрайта
		apple.sprite.setTexture(game.appletexture);
		SetSpriteSize(apple.sprite, APPLE_SIZE, APPLE_SIZE);
		SetSpriteRelativeOrigine(apple.sprite, 0.5f, 0.5f);
	}

	void DrawApple(Apple& apple, sf::RenderWindow& window)
	{
		apple.sprite.setPosition(apple.Position.x, apple.Position.y);
		window.draw(apple.sprite);
	}
}