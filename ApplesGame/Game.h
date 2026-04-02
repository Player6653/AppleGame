#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Math.h"
#include "Constants.h"
#include "Apple.h"
#include "Player.h"
#include "Rock.h"

namespace APPLE_GAME
{
	struct Game
	{
		Player player;
		Apple apples[NUM_APPLES];
		Rock rocks[NUM_ROCKS];

		// Глобальные сведения игры
		int numEatenApples = 0;
		bool isGameFinished = false;
		float gameFinishTime = 0.f;
		sf::RectangleShape background;

		// Ресурсы
		sf::Texture playertexture;
		sf::Texture appletexture;
		sf::Texture rocktexture;

		// Саунд
		sf::SoundBuffer eatAppleSoundBuffer;
		sf::SoundBuffer deathSoundBuffer;
		sf::Sound eatAppleSound;
		sf::Sound deathSound;

		// интерфейс 
		sf::Font font;
		sf::Text scoreText;
		sf::Text hintText;
		sf::Text gameOverText;

		bool isHintVisible = true; // таймер управления
	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	bool CheckCollisionApple(const Player& player, const Apple& apple);
	bool CheckCollisionRock(const Player& player, const Rock& rock);
	bool CheckPlayerScreenCollision(const Player& player);
	void UpdatePlayingState(Game& game, float deltaTime, float currentTime);
	void UpdateGameOverState(Game& game, float currentTime);
	void UpdateGame(Game& game, float deltaTime, float currentTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinitGame(Game& game);
}