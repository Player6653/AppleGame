#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include "Constants.h"
#include "Game.h"

int main()
{
	using namespace APPLE_GAME;
	int seed = (int)time(nullptr);
	srand(seed);
	// Инициализация окна
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apple Game");

	// Инициализация времени игры
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	Game game;
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

		// Ивент закрытия
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				game.isGameFinished = true;
				game.gameFinishTime = currentTime;
			}
		}

		if (!game.isGameFinished)
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
		UpdateGame(game, deltaTime, currentTime);
		window.clear();
		DrawGame(game, window);
		window.display();
	}
	DeinitGame(game);
	return 0;
}