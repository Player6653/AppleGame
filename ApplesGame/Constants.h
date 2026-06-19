#pragma once
#include <string>
#include <iostream>

namespace APPLE_GAME
{
	const std::string RESOURCES_PATH = "Resources/";
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_SPEED = 100.f; // ������� � �������
	const float PLAYER_SIZE = 20.f;
	const float ACCELERATION = 20.f; // �����������
	const float APPLE_SIZE = 20.f;
	const float PAUSE_LENGTH = 1.2f;
	const int NUM_ROCKS = 10;
	const float ROCK_SIZE = 25.f;

	const unsigned int GAME_MODE_INFINITE_APPLES   = 1 << 0;
	const unsigned int GAME_MODE_WITH_ACCELERATION = 1 << 1;
}