#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"

int main()
{
	using namespace APPLE_GAME;

	int seed = (int)time(nullptr);
	srand(seed);

	Game game;
	game.numApples = 20;
	game.gameMode = GAME_MODE_INFINITE_APPLES | GAME_MODE_WITH_ACCELERATION;

	// Инициализация окна
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apple Game");

	// Инициализация времени игры
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	InitGame(game);

	// Главный цикл
	while (window.isOpen())
	{
		// Задержка рендеринга
		sf::sleep(sf::milliseconds(8));

		// Дельта времени
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Обработка событий
		bool resumedFromPause = false;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				// Диалог выхода имеет приоритет над всеми
				if (game.isShowingExitDialog)
				{
					if (event.key.code == sf::Keyboard::Y || event.key.code == sf::Keyboard::Return)
					{
						window.close();
						break;
					}
					else if (event.key.code == sf::Keyboard::N || event.key.code == sf::Keyboard::Escape)
					{
						game.isShowingExitDialog = false;
					}
					continue;
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					game.isShowingExitDialog = true;
					continue;
				}
			}

			if (game.state == GameState::ModeSelect)
			{
				if (event.type == sf::Event::KeyPressed)
				{
					MenuAction action = HandleModeSelectInput(game, event.key.code);
					if (action == MenuAction::StartGame)
					{
						StartGame(game, currentTime);
						lastTime = gameClock.getElapsedTime().asSeconds();
					}
					else if (action == MenuAction::ShowLeaderboard)
					{
						ShowLeaderboardFromMenu(game);
					}
				}
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::R:
					if (game.state == GameState::Leaderboard)
					{
						game.state = GameState::Playing;
						game.isHintVisible = true;
						game.hintStartTime = currentTime;
						RestartGame(game);
					}
					else if (game.state == GameState::Playing)
					{
						game.state = GameState::GameOver;
						game.gameFinishTime = currentTime;
					}
					break;
				case sf::Keyboard::P:
					if (game.state == GameState::Playing)
					{
						game.pauseEnteredTime = currentTime;
						EnterPauseMenu(game);
					}
					else if (game.state == GameState::PauseMenu)
					{
						game.hintStartTime += currentTime - game.pauseEnteredTime;
						game.state = GameState::Playing;
						resumedFromPause = true;
					}
					else
						ReturnToModeSelect(game);
					break;
				case sf::Keyboard::BackSpace:
					ReturnToModeSelect(game);
					break;
				case sf::Keyboard::Up:
					if (game.state == GameState::PauseMenu && game.pauseMenuCursor > 0)
						--game.pauseMenuCursor;
					break;
				case sf::Keyboard::Down:
					if (game.state == GameState::PauseMenu && game.pauseMenuCursor < 1)
						++game.pauseMenuCursor;
					break;
				case sf::Keyboard::Return:
					if (game.state == GameState::PauseMenu)
					{
						if (game.pauseMenuCursor == 0)
						{
							game.hintStartTime += currentTime - game.pauseEnteredTime;
							game.state = GameState::Playing;
							resumedFromPause = true;
						}
						else
							ReturnToModeSelect(game);
					}
					break;
				default:
					break;
				}
			}
		}

		if (game.state == GameState::Playing && !game.isShowingExitDialog && !resumedFromPause)
		{
			// Обработка нажатий
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				game.player.Direction = PlayerDirection::Right;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				game.player.Direction = PlayerDirection::Up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				game.player.Direction = PlayerDirection::Left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				game.player.Direction = PlayerDirection::Down;
			}
		}

		if (game.state != GameState::ModeSelect && game.state != GameState::PauseMenu && !game.isShowingExitDialog)
		{
			UpdateGame(game, deltaTime, currentTime);
		}

		window.clear();
		DrawGame(game, window);
		window.display();
	}
	DeinitGame(game);
	return 0;
}
