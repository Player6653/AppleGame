#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

const std::string RESOURCES_PATH = "Resources/";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float INITIAL_SPEED = 100.f; // пиксели в секунду
const float PLAYER_SIZE = 20.f;
const float ACCELERATION = 20.f; // акселерация
const int NUM_APPLES = 20;
const float APPLE_SIZE = 20.f;
const float PAUSE_LENGTH = 0.5f;
const int NUM_ROCKS = 10;
const float ROCK_SIZE = 15.f;

struct Vector2D
{
	float x = 0;
	float y = 0;
};

enum class PlayerDirection
{
	Zero = 0,
	Right,
	Up,
	Left,
	Down
};

typedef Vector2D Position2D;

Position2D GetRandomPositionInScreen()
{
	Position2D result;
	result.x = rand() / (float)RAND_MAX * SCREEN_WIDTH;
	result.y = rand() / (float)RAND_MAX * SCREEN_HEIGHT;
	return result;
}

bool IsRectanglesCoolide(Position2D rect1Position, Vector2D rect1Size, Position2D rect20rigin, Vector2D rect2Size);

struct GameState
{
	// Сведения игрока
	Position2D playerPosition;
	float playerSpeed = INITIAL_SPEED;
	PlayerDirection playerDirection = PlayerDirection::Zero;
	sf::RectangleShape playerShape;

	// Сведения яблок
	Position2D applesPositions[NUM_APPLES];
	sf::CircleShape applesShape[NUM_APPLES];

	// Сведения камней
	Position2D rocksPositions[NUM_ROCKS];
	sf::RectangleShape rocksShape[NUM_ROCKS];

	// Глобальные сведения игры
	int numEatenApples = 0;
	bool isGameFinished = false;
	float gameFinishTime = 0.f;
	sf::RectangleShape background;
};

void InitGame(GameState& gameState)
{
	// Инициализация статов игрока
	gameState.playerPosition = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
	gameState.playerSpeed = INITIAL_SPEED;
	gameState.playerDirection = PlayerDirection::Zero;

	// Инициализация плеершейп
	gameState.playerShape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
	gameState.playerShape.setFillColor(sf::Color::Red);
	gameState.playerShape.setOrigin(PLAYER_SIZE / 2.f, PLAYER_SIZE / 2.f);
	gameState.playerShape.setPosition(gameState.playerPosition.x, gameState.playerPosition.y);

	for (int i = 0; i < NUM_APPLES; ++i)
	{
		gameState.applesPositions[i] = GetRandomPositionInScreen();

		gameState.applesShape[i].setRadius(APPLE_SIZE / 2.f);
		gameState.applesShape[i].setFillColor(sf::Color::Green);
		gameState.applesShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
		gameState.applesShape[i].setPosition(gameState.applesPositions[i].x, gameState.applesPositions[i].y);
	}

	// Инициализация камней
	for (int i = 0; i < NUM_ROCKS; ++i)
	{
		gameState.rocksPositions[i] = GetRandomPositionInScreen();

		gameState.rocksShape[i].setSize(sf::Vector2f(ROCK_SIZE, ROCK_SIZE));
		gameState.rocksShape[i].setFillColor(sf::Color::Yellow);
		gameState.rocksShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
		gameState.rocksShape[i].setPosition(gameState.rocksPositions[i].x, gameState.rocksPositions[i].y);
	}

	gameState.numEatenApples = 0;
	gameState.isGameFinished = false;
	gameState.gameFinishTime = 0.f;

	gameState.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	gameState.background.setFillColor(sf::Color::Black);
	gameState.background.setPosition(0.f, 0.f);
}

void UpdateGame(GameState& gameState, float deltaTime, float currentTime)
{
	if (!gameState.isGameFinished)
	{
		// Обработка нажатий
		switch (gameState.playerDirection)
		{
		case PlayerDirection::Right:
			gameState.playerPosition.x += gameState.playerSpeed * deltaTime;
			break;
		case PlayerDirection::Up:
			gameState.playerPosition.y -= gameState.playerSpeed * deltaTime;
			break;
		case PlayerDirection::Left:
			gameState.playerPosition.x -= gameState.playerSpeed * deltaTime;
			break;
		case PlayerDirection::Down:
			gameState.playerPosition.y += gameState.playerSpeed * deltaTime;
			break;
		}

	// Проверка колизий яболк
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			float squareDistance = (gameState.playerPosition.x - gameState.applesPositions[i].x) * (gameState.playerPosition.x - gameState.applesPositions[i].x) +
				(gameState.playerPosition.y - gameState.applesPositions[i].y) * (gameState.playerPosition.y - gameState.applesPositions[i].y);
			float squareRadiusSum = (APPLE_SIZE + PLAYER_SIZE) * (APPLE_SIZE + PLAYER_SIZE) / 4;
			if (squareDistance <= squareRadiusSum)
			{
				gameState.applesPositions[i] = GetRandomPositionInScreen();
				++gameState.numEatenApples;
				gameState.playerSpeed += ACCELERATION;
			}
		}

		// Проверка колизий камней
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			float dx = fabs(gameState.playerPosition.x - gameState.rocksPositions[i].x);
			float dy = fabs(gameState.playerPosition.y - gameState.rocksPositions[i].y);
			if (dx <= (ROCK_SIZE + PLAYER_SIZE) / 2.f &&
				dy <= (ROCK_SIZE + PLAYER_SIZE) / 2.f)
			{
				gameState.isGameFinished = true;
				gameState.gameFinishTime = currentTime;
			}
		}

		// Проверка колизий границ экрана
		if (gameState.playerPosition.x - PLAYER_SIZE / 2.f < 0.f || gameState.playerPosition.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
			gameState.playerPosition.y - PLAYER_SIZE / 2.f < 0.f || gameState.playerPosition.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
		{
			gameState.isGameFinished = true;
			gameState.gameFinishTime = currentTime;
		}
	}
	else
	{
		if (currentTime - gameState.gameFinishTime <= PAUSE_LENGTH)
		{
			gameState.background.setFillColor(sf::Color::Red);
		}
		else
		{
			InitGame(gameState);
		}
	}
}

void DrawGame(GameState& gameState, sf::RenderWindow& window, sf::Text& scoreText)
{
	window.draw(gameState.background);
	gameState.playerShape.setPosition(gameState.playerPosition.x, gameState.playerPosition.y);
	for (int i = 0; i < NUM_APPLES; ++i)
	{
		gameState.applesShape[i].setPosition(gameState.applesPositions[i].x, gameState.applesPositions[i].y);
		window.draw(gameState.applesShape[i]);
	}

	for (int i = 0; i < NUM_ROCKS; ++i)
	{
		gameState.rocksShape[i].setPosition(gameState.rocksPositions[i].x, gameState.rocksPositions[i].y);
		window.draw(gameState.rocksShape[i]);
	}
	scoreText.setString(L"Счет: " + std::to_wstring(gameState.numEatenApples));
	window.draw(scoreText);
	window.draw(gameState.playerShape);
}

void DeinitGame(GameState& gameState)
{

}


int main()
{
	int seed = (int)time(nullptr);
	srand(seed);
	// Инициализация окна
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

	// Инициализация времени игры
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	GameState gameState;
	InitGame(gameState);

	// Инициализация шрифта и текста
	sf::Font font;
	if (!font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"))
	{
		std::cout << "Ошибка загрузки шрифта!\n";
	}
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10.f, 10.f);

	// Главный цикл
	while (window.isOpen())
	{
		// Дельта времнеи
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
				gameState.isGameFinished = true;
				gameState.gameFinishTime = currentTime;
			}
		}

		if (!gameState.isGameFinished)
		{
			// Обработка нажатий
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				gameState.playerDirection = PlayerDirection::Right;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				gameState.playerDirection = PlayerDirection::Up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				gameState.playerDirection = PlayerDirection::Left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				gameState.playerDirection = PlayerDirection::Down;
			}
		}
		UpdateGame(gameState, deltaTime, currentTime);
		window.clear();
		DrawGame(gameState, window, scoreText);
		window.display();
	}

	DeinitGame(gameState);

	return 0;
}