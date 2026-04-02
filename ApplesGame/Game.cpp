#include "Game.h"
#include <cassert>

namespace APPLE_GAME
{
	void RestartGame(Game& game)
	{
		InitPlayer(game.player, game);
		game.scoreText.setString(L"Счет: 0");

		for (int i = 0; i < NUM_APPLES; ++i)
		{
			InitApples(game.apples[i], game);
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRocks(game.rocks[i], game);
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.gameFinishTime = 0.f;

		game.background.setFillColor(sf::Color::Black);
	}

	void InitGame(Game& game)
	{
		// Спрайты
		assert(game.playertexture.loadFromFile(RESOURCES_PATH + "Pictures/Player.png"));
		assert(game.appletexture.loadFromFile(RESOURCES_PATH + "Pictures/Apple.png"));
		assert(game.rocktexture.loadFromFile(RESOURCES_PATH + "Pictures/Rock.png"));
		
		// Звуки
		assert(game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "Sound/AppleEat.wav"));
		assert(game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Sound/Death.wav"));

		game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
		game.deathSound.setBuffer(game.deathSoundBuffer);

		// Интерфейс
		assert(game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"));

		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::Yellow);
		game.scoreText.setPosition(10.f, 10.f);

		game.hintText.setFont(game.font);
		game.hintText.setCharacterSize(22);
		game.hintText.setFillColor(sf::Color::White);
		game.hintText.setString(L"Управление: \nСтрелочки/WASD \nR – Перезагрузка \nESC – Выход");
		game.hintText.setPosition(10.f, 40.f);

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(72);
		game.gameOverText.setFillColor(sf::Color::White);
		game.gameOverText.setStyle(sf::Text::Bold);
		game.gameOverText.setString(L"Потрачено");

		sf::FloatRect textRect = game.gameOverText.getLocalBounds();
		game.gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		
		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		RestartGame(game);
	}

	bool CheckCollisionApple(const Player& player, const Apple& apple)
	{
		return IsCirclesCollide(player.Position, PLAYER_SIZE / 2.f, apple.Position, APPLE_SIZE / 2.f);
	}

	bool CheckCollisionRock(const Player& player, const Rock& rock)
	{
		return IsRectanglesCoolide(player.Position, { PLAYER_SIZE, PLAYER_SIZE }, rock.Position, { ROCK_SIZE, ROCK_SIZE });
	}

	bool CheckPlayerScreenCollision(const Player& player)
	{
		return player.Position.x - PLAYER_SIZE / 2.f < 0.f || player.Position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
			player.Position.y - PLAYER_SIZE / 2.f < 0.f || player.Position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT;
	}

	void UpdatePlayingState(Game& game, float deltaTime, float currentTime)
	{
		UpdatePlayer(game.player, deltaTime);

		// Проверка колизий яболк
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			if (CheckCollisionApple(game.player, game.apples[i]))
			{
				InitApples(game.apples[i], game);
				++game.numEatenApples;
				game.eatAppleSound.play();
				game.player.Speed += ACCELERATION;
			}
		}

		// Проверка колизий камней
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			if (CheckCollisionRock(game.player, game.rocks[i]))
			{
				game.deathSound.play();
				game.isGameFinished = true;
				game.gameFinishTime = currentTime;
			}
		}

		// Проверка колизий границ экрана
		if (CheckPlayerScreenCollision(game.player))
		{
			game.deathSound.play();
			game.isGameFinished = true;
			game.gameFinishTime = currentTime;
		}

		if (currentTime > 5.f)
		{
			game.isHintVisible = false;
		}
	}

	void UpdateGameOverState(Game& game, float currentTime)
	{
		if (currentTime - game.gameFinishTime <= PAUSE_LENGTH)
		{
			game.background.setFillColor(sf::Color::Red);
		}
		else
		{
			RestartGame(game);
		}
	}

	void UpdateGame(Game& game, float deltaTime, float currentTime)
	{
		if (!game.isGameFinished)
		{
			UpdatePlayingState(game, deltaTime, currentTime);
			game.scoreText.setString(L"Счет: " + std::to_wstring(game.numEatenApples));
		}
		else
		{
			UpdateGameOverState(game, currentTime);
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);
		DrawPlayer(game.player, window);
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			DrawApple(game.apples[i], window);
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			DrawRock(game.rocks[i], window);
		}

		window.draw(game.scoreText);
		if (game.isHintVisible)
		{
			window.draw(game.hintText);
		}

		if (game.isGameFinished)
		{
			window.draw(game.gameOverText);
		}
	}

	void DeinitGame(Game& game)
	{

	}
}