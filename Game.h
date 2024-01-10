#pragma once
#include "Entity.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

class Game {
	int wWidth = 800;
    int wHeight = 600;

	const float playerWidth = 22.f;
	const float playerHeight = 100.f;
	const float ballRadius = 7.f;

	sf::RenderWindow window;

	std::shared_ptr<Entity>player1;
	std::shared_ptr<Entity>player2;
	std::shared_ptr<Entity>ball;

	sf::Font font;
	sf::Text scoreText;
	sf::Text announceText;

	sf::SoundBuffer buffer;
	sf::Sound pongSound;

	const int maxScore = 5;
	int p1Score = 0;
	int p2Score = 0;

	bool isRunning = true;
	bool isPlaying = false;
	bool isPaused = false;
	bool victory = false;

	void setRound();

public:
	Game();
	void run();
};
