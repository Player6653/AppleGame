#include <SFML/Graphics.hpp>

int main()
{
	// Инициализация окна
	sf::RenderWindow window(sf::VideoMode(800, 600), "Tetris");

	// Главный цикл
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);
		window.display();
	}

	return 0;
}
