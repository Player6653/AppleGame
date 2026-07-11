#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include "Math.h"
#include "Constants.h"
#include "Apple.h"
#include "Player.h"
#include "Rock.h"

namespace APPLE_GAME
{
	enum class GameState
	{
		ModeSelect,
		Playing,
		GameOver,
		GameWon,
		Leaderboard,
		PauseMenu
	};

	enum class MenuAction
	{
		None,
		StartGame,
		ShowLeaderboard
	};

	struct Record
	{
		std::wstring name;
		int score;
	};

	struct Game
	{
		Player player;
		std::vector<Apple> apples;
		int numApples = 0;
		unsigned int gameMode = 0;
		Rock rocks[NUM_ROCKS];

		int numEatenApples = 0;
		GameState state = GameState::ModeSelect;
		int modeSelectCursor = 0;
		int pauseMenuCursor = 0;
		float gameFinishTime = 0.f;
		float hintStartTime = 0.f;
		float pauseEnteredTime = 0.f;
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
		sf::Text leaderboardText;

		std::unordered_map<std::wstring, int> leaderboard;

		bool isHintVisible = true;
		bool isShowingExitDialog = false;

		sf::RectangleShape exitDialogBackground;
		sf::Text exitDialogText;

		sf::RectangleShape pauseMenuBackground;
		sf::Text pauseMenuContinueText;
		sf::Text pauseMenuExitText;
	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	void StartGame(Game& game, float currentTime);
	void ReturnToModeSelect(Game& game);
	void ShowLeaderboardFromMenu(Game& game);
	void EnterPauseMenu(Game& game);
	MenuAction HandleModeSelectInput(Game& game, sf::Keyboard::Key key);
	bool CheckCollisionApple(const Player& player, const Apple& apple);
	bool CheckCollisionRock(const Player& player, const Rock& rock);
	bool CheckPlayerScreenCollision(const Player& player);
	void UpdatePlayingState(Game& game, float deltaTime, float currentTime);
	void UpdateGameOverState(Game& game, float currentTime);
	void UpdateGame(Game& game, float deltaTime, float currentTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinitGame(Game& game);
}