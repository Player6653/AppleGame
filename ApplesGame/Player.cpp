#include "Player.h"
#include "Game.h"

namespace APPLE_GAME
{
	void InitPlayer(Player& player, const Game& game)
	{
		// Инициализация игрока
		player.Position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
		player.Speed = INITIAL_SPEED;
		player.Direction = PlayerDirection::Zero;

		// Инициализация спрайта
		player.sprite.setTexture(game.playertexture);
		SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
		SetSpriteRelativeOrigine(player.sprite, 0.5f, 0.5f);
	}

	void DrawPlayer(Player& player, sf::RenderWindow& window)
	{
		player.sprite.setPosition(player.Position.x, player.Position.y);
		window.draw(player.sprite);
	}

	void UpdatePlayer(Player& player, float deltaTime)
	{
		sf::Vector2f scale = player.sprite.getScale();
		float scaleX = (float)fabs(scale.x);
		float scaleY = (float)fabs(scale.y);

		// Обработка нажатий
		switch (player.Direction)
		{
		case PlayerDirection::Right:
			player.Position.x += player.Speed * deltaTime;
			player.sprite.setRotation(0.f);
			player.sprite.setScale(scaleX, scaleY);
			break;
		case PlayerDirection::Up:
			player.Position.y -= player.Speed * deltaTime;
			player.sprite.setRotation(-90.f);
			player.sprite.setScale(scaleX, scaleY);
			break;
		case PlayerDirection::Left:
			player.Position.x -= player.Speed * deltaTime;
			player.sprite.setRotation(0.f);
			player.sprite.setScale(-scaleX, scaleY);
			break;
		case PlayerDirection::Down:
			player.Position.y += player.Speed * deltaTime;
			player.sprite.setRotation(90.f);
			player.sprite.setScale(scaleX, -scaleY);
			break;
		}
	}
}