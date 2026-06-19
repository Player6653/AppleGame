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
		Apple* apples = nullptr;
		int numApples = 0;
		unsigned int gameMode = 0;
		Rock rocks[NUM_ROCKS];

		int numEatenApples = 0;
		bool isGameFinished = false;
		bool isGameWon = false;
		bool isSelectingMode = true;
		float gameFinishTime = 0.f;
		float hintStartTime = 0.f;
		sf::RectangleShape background;

		sf::Texture playertexture;
		sf::Texture appletexture;
		sf::Texture rocktexture;

		sf::SoundBuffer eatAppleSoundBuffer;
		sf::SoundBuffer deathSoundBuffer;
		sf::Sound eatAppleSound;
		sf::Sound deathSound;

		sf::Font font;
		sf::Text scoreText;
		sf::Text hintText;
		sf::Text gameOverText;
		sf::Text gameWonText;
		sf::Text modeSelectText;

		bool isHintVisible = true;
	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	void StartGame(Game& game, float currentTime);
	void ReturnToModeSelect(Game& game);
	bool HandleModeSelectInput(Game& game, sf::Keyboard::Key key);
	bool CheckCollisionApple(const Player& player, const Apple& apple);
	bool CheckCollisionRock(const Player& player, const Rock& rock);
	bool CheckPlayerScreenCollision(const Player& player);
	void UpdatePlayingState(Game& game, float deltaTime, float currentTime);
	void UpdateGameOverState(Game& game, float currentTime);
	void UpdateGame(Game& game, float deltaTime, float currentTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinitGame(Game& game);
}