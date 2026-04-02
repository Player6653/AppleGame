#include "Rock.h"
#include "Game.h"

namespace APPLE_GAME
{
	void InitRocks(Rock& rock, const Game& game)
	{
		rock.Position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		// Инициализация спрайта
		rock.sprite.setTexture(game.rocktexture);
		SetSpriteSize(rock.sprite, ROCK_SIZE, ROCK_SIZE);
		SetSpriteRelativeOrigine(rock.sprite, 0.5f, 0.5f);
	}

	void DrawRock(Rock& rock, sf::RenderWindow& window)
	{
		rock.sprite.setPosition(rock.Position.x, rock.Position.y);
		window.draw(rock.sprite);
	}

}