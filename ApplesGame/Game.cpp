#include "Game.h"
#include <cassert>
#include <string>

namespace APPLE_GAME
{
	static void RebuildModeSelectText(Game& game)
	{
		std::wstring text = L"=== ВЫБОР РЕЖИМА ===\n\n";
		text += L"[1]  Яблоки: ";
		text += (game.gameMode & GAME_MODE_INFINITE_APPLES) ? L"БЕСКОНЕЧНЫЕ" : L"КОНЕЧНЫЕ";
		text += L"\n[2]  Ускорение: ";
		text += (game.gameMode & GAME_MODE_WITH_ACCELERATION) ? L"ВКЛ" : L"ВЫКЛ";
		text += L"\n[вверх / вниз]  Количество яблок: ";
		text += std::to_wstring(game.numApples);
		text += L"\n\n[Enter]  Начать игру";
		game.modeSelectText.setString(text);
	}

	void RestartGame(Game& game)
	{
		InitPlayer(game.player, game);
		game.scoreText.setString(L"Счет: 0");

		for (int i = 0; i < game.numApples; ++i)
		{
			InitApples(game.apples[i], game);
			game.apples[i].isEaten = false;
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRocks(game.rocks[i], game);
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.isGameWon = false;
		game.gameFinishTime = 0.f;

		game.background.setFillColor(sf::Color::Black);
	}

	void InitGame(Game& game)
	{
		assert(game.playertexture.loadFromFile(RESOURCES_PATH + "Pictures/Player.png"));
		assert(game.appletexture.loadFromFile(RESOURCES_PATH + "Pictures/Apple.png"));
		assert(game.rocktexture.loadFromFile(RESOURCES_PATH + "Pictures/Rock.png"));

		game.apples = new Apple[game.numApples];

		assert(game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "Sound/AppleEat.wav"));
		assert(game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Sound/Death.wav"));

		game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
		game.deathSound.setBuffer(game.deathSoundBuffer);

		assert(game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"));

		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::Yellow);
		game.scoreText.setPosition(10.f, 10.f);

		game.hintText.setFont(game.font);
		game.hintText.setCharacterSize(22);
		game.hintText.setFillColor(sf::Color::White);
		game.hintText.setString(L"Управление: \nСтрелочки/WASD \nR – Перезагрузка \nP – Выбор режима \nESC – Выход");
		game.hintText.setPosition(10.f, 40.f);

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(72);
		game.gameOverText.setFillColor(sf::Color::White);
		game.gameOverText.setStyle(sf::Text::Bold);
		game.gameOverText.setString(L"Потрачено");

		sf::FloatRect textRect = game.gameOverText.getLocalBounds();
		game.gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

		game.gameWonText.setFont(game.font);
		game.gameWonText.setCharacterSize(72);
		game.gameWonText.setFillColor(sf::Color::White);
		game.gameWonText.setStyle(sf::Text::Bold);
		game.gameWonText.setString(L"Победа!");

		sf::FloatRect wonTextRect = game.gameWonText.getLocalBounds();
		game.gameWonText.setOrigin(wonTextRect.left + wonTextRect.width / 2.0f, wonTextRect.top + wonTextRect.height / 2.0f);
		game.gameWonText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

		game.modeSelectText.setFont(game.font);
		game.modeSelectText.setCharacterSize(28);
		game.modeSelectText.setFillColor(sf::Color::White);
		game.modeSelectText.setPosition(40.f, 140.f);
		RebuildModeSelectText(game);

		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		RestartGame(game);
	}

	void StartGame(Game& game, float currentTime)
	{
		delete[] game.apples;
		game.apples = new Apple[game.numApples];
		game.isSelectingMode = false;
		game.hintStartTime = currentTime;
		game.isHintVisible = true;
		RestartGame(game);
	}

	void ReturnToModeSelect(Game& game)
	{
		game.isSelectingMode = true;
		game.isGameFinished = false;
		game.isGameWon = false;
		game.background.setFillColor(sf::Color::Black);
	}

	bool HandleModeSelectInput(Game& game, sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Num1:
			game.gameMode ^= GAME_MODE_INFINITE_APPLES;
			RebuildModeSelectText(game);
			break;
		case sf::Keyboard::Num2:
			game.gameMode ^= GAME_MODE_WITH_ACCELERATION;
			RebuildModeSelectText(game);
			break;
		case sf::Keyboard::Up:
			if (game.numApples < 100) game.numApples += 5;
			RebuildModeSelectText(game);
			break;
		case sf::Keyboard::Down:
			if (game.numApples > 5) game.numApples -= 5;
			RebuildModeSelectText(game);
			break;
		case sf::Keyboard::Return:
			return true;
		}
		return false;
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

		for (int i = 0; i < game.numApples; ++i)
		{
			if (!game.apples[i].isEaten && CheckCollisionApple(game.player, game.apples[i]))
			{
				if (game.gameMode & GAME_MODE_INFINITE_APPLES)
				{
					InitApples(game.apples[i], game);
				}
				else
				{
					game.apples[i].isEaten = true;
				}
				++game.numEatenApples;
				game.eatAppleSound.play();
				if (game.gameMode & GAME_MODE_WITH_ACCELERATION)
				{
					game.player.Speed += ACCELERATION;
				}
			}
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			if (CheckCollisionRock(game.player, game.rocks[i]))
			{
				game.deathSound.play();
				game.isGameFinished = true;
				game.gameFinishTime = currentTime;
				break;
			}
		}

		if (!game.isGameFinished && CheckPlayerScreenCollision(game.player))
		{
			game.deathSound.play();
			game.isGameFinished = true;
			game.gameFinishTime = currentTime;
		}

		if (!game.isGameFinished && !(game.gameMode & GAME_MODE_INFINITE_APPLES) && game.numEatenApples >= game.numApples)
		{
			game.isGameFinished = true;
			game.isGameWon = true;
			game.gameFinishTime = currentTime;
		}

		if (currentTime - game.hintStartTime > 5.f)
		{
			game.isHintVisible = false;
		}
	}

	void UpdateGameOverState(Game& game, float currentTime)
	{
		if (currentTime - game.gameFinishTime <= PAUSE_LENGTH)
		{
			game.background.setFillColor(game.isGameWon ? sf::Color::Green : sf::Color::Red);
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

		if (game.isSelectingMode)
		{
			window.draw(game.modeSelectText);
			return;
		}

		DrawPlayer(game.player, window);
		for (int i = 0; i < game.numApples; ++i)
		{
			if (!game.apples[i].isEaten)
			{
				DrawApple(game.apples[i], window);
			}
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
			window.draw(game.isGameWon ? game.gameWonText : game.gameOverText);
		}
	}

	void DeinitGame(Game& game)
	{
		delete[] game.apples;
		game.apples = nullptr;
	}
}