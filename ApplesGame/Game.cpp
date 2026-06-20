#include "Game.h"
#include <string>
#include <vector>
#include <utility>

namespace APPLE_GAME
{
	static const std::wstring FAKE_NAMES[] = {
		L"Алиса", L"Владислав", L"Глеб", L"Дарья", L"Ева",
		L"Никита", L"Олег", L"Сандра", L"Федор", L"Ян"
	};
	static const int NUM_FAKE_NAMES = 10;

	static void SortEntriesDescending(std::vector<std::pair<std::wstring, int>>& entries)
	{
		// Insertion sort по убыванию очков
		for (int i = 1; i < (int)entries.size(); ++i)
		{
			auto key = entries[i];
			int j = i - 1;
			while (j >= 0 && entries[j].second < key.second)
			{
				entries[j + 1] = entries[j];
				--j;
			}
			entries[j + 1] = key;
		}
	}

	static void UpdateLeaderboardText(Game& game)
	{
		std::vector<std::pair<std::wstring, int>> entries;
		for (auto& kv : game.leaderboard)
			entries.push_back(kv);

		SortEntriesDescending(entries);

		const int NAME_COL = 16;
		std::wstring text = L"===== Доска Почета =====\n";
		for (int i = 0; i < (int)entries.size(); ++i)
		{
			const std::wstring& name = entries[i].first;
			std::wstring line = std::to_wstring(i + 1) + L". " + name + L" ";
			int dots = NAME_COL - (int)name.size() - 1;
			if (dots < 2) dots = 2;
			for (int d = 0; d < dots; ++d) line += L".";
			line += L" " + std::to_wstring(entries[i].second) + L"\n";
			text += line;
		}
		text += L"========================\n";
		text += L"\n[R] Перезапуск   [P] Выбор режима";

		game.leaderboardText.setString(text);
	}

	static void InitLeaderboard(Game& game)
	{
		game.leaderboard.clear();

		std::wstring shuffled[NUM_FAKE_NAMES];
		for (int i = 0; i < NUM_FAKE_NAMES; ++i)
			shuffled[i] = FAKE_NAMES[i];
		for (int i = NUM_FAKE_NAMES - 1; i > 0; --i)
		{
			int j = rand() % (i + 1);
			std::wstring tmp = shuffled[i];
			shuffled[i] = shuffled[j];
			shuffled[j] = tmp;
		}

		int count = 6 + rand() % 3;
		int scoreRange = (game.gameMode & GAME_MODE_INFINITE_APPLES)
			? game.numApples * 3 + 15
			: game.numApples;
		for (int i = 0; i < count; ++i)
			game.leaderboard[shuffled[i]] = rand() % scoreRange;

		game.leaderboard[L"Игрок"] = 0;
		UpdateLeaderboardText(game);
	}

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
			do
			{
				InitRocks(game.rocks[i], game);
			} while (IsRectanglesCoolide(game.player.Position, { PLAYER_SIZE, PLAYER_SIZE },
				game.rocks[i].Position, { ROCK_SIZE, ROCK_SIZE }));
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.isGameWon = false;
		game.gameFinishTime = 0.f;

		game.background.setFillColor(sf::Color::Black);
	}

	void InitGame(Game& game)
	{
		if (!game.playertexture.loadFromFile(RESOURCES_PATH + "Pictures/Player.png") ||
			!game.appletexture.loadFromFile(RESOURCES_PATH + "Pictures/Apple.png") ||
			!game.rocktexture.loadFromFile(RESOURCES_PATH + "Pictures/Rock.png"))
		{
			std::cout << "Ошибка загрузки текстур!\n";
			exit(1);
		}

		game.apples.resize(game.numApples);

		if (!game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "Sound/AppleEat.wav") ||
			!game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Sound/Death.wav"))
		{
			std::cout << "Ошибка загрузки звуков!\n";
			exit(1);
		}

		game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
		game.deathSound.setBuffer(game.deathSoundBuffer);

		if (!game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"))
		{
			std::cout << "Ошибка загрузки шрифта!\n";
			exit(1);
		}

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

		game.leaderboardText.setFont(game.font);
		game.leaderboardText.setCharacterSize(28);
		game.leaderboardText.setFillColor(sf::Color::White);
		game.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - 280.f, SCREEN_HEIGHT / 2.f - 240.f);

		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		game.exitDialogBackground.setSize(sf::Vector2f(420.f, 160.f));
		game.exitDialogBackground.setFillColor(sf::Color(30, 30, 30, 230));
		game.exitDialogBackground.setOutlineColor(sf::Color::White);
		game.exitDialogBackground.setOutlineThickness(2.f);
		game.exitDialogBackground.setPosition(SCREEN_WIDTH / 2.f - 210.f, SCREEN_HEIGHT / 2.f - 80.f);

		game.exitDialogText.setFont(game.font);
		game.exitDialogText.setCharacterSize(24);
		game.exitDialogText.setFillColor(sf::Color::White);
		game.exitDialogText.setString(L"Хотите выйти?\n\n[Y/Enter] Да       [N/Esc] Нет");
		game.exitDialogText.setPosition(SCREEN_WIDTH / 2.f - 190.f, SCREEN_HEIGHT / 2.f - 65.f);

		RestartGame(game);
	}

	void StartGame(Game& game, float currentTime)
	{
		game.apples.resize(game.numApples);
		game.isSelectingMode = false;
		game.hintStartTime = currentTime;
		game.isHintVisible = true;
		InitLeaderboard(game);
		RestartGame(game);
	}

	void ReturnToModeSelect(Game& game)
	{
		game.isSelectingMode = true;
		game.isGameFinished = false;
		game.isGameWon = false;
		game.isShowingLeaderboard = false;
		game.isShowingExitDialog = false;
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
		else if (!game.isShowingLeaderboard)
		{
			game.leaderboard[L"Игрок"] = game.numEatenApples;
			UpdateLeaderboardText(game);
			game.background.setFillColor(sf::Color::Black);
			game.isShowingLeaderboard = true;
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
		}
		else if (game.isShowingLeaderboard)
		{
			window.draw(game.leaderboardText);
		}
		else
		{
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

		if (game.isShowingExitDialog)
		{
			window.draw(game.exitDialogBackground);
			window.draw(game.exitDialogText);
		}
	}

	void DeinitGame(Game& game)
	{
	}
}