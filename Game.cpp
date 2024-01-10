#include "Game.h"
#include<string>

Game::Game(){
	srand(time(0));
	window.create(sf::VideoMode(wWidth, wHeight), "PingPong", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	player1 = std::shared_ptr<Entity>(new Entity(0, "player1"));
	player1->cTransform = std::make_shared<CTransform>(Vec2(playerWidth / 2 + 8.f, wHeight / 2.f), Vec2(0.f, 0.f));
	player1->cShape = std::make_shared<CShape>(playerWidth, playerHeight, sf::Color::Red, sf::Color::Black, 3.f);
	player1->cInput = std::make_shared<CInput>();

	player2= std::shared_ptr<Entity>(new Entity(1, "player2"));
	player2->cTransform = std::make_shared<CTransform>(Vec2(wWidth - playerWidth / 2 - 8.f, wHeight / 2.f), Vec2(0.f, 0.f));
	player2->cShape = std::make_shared<CShape>(playerWidth, playerHeight, sf::Color::Blue, sf::Color::Black, 3.f);
	player2->cInput = std::make_shared<CInput>();

	ball = std::shared_ptr<Entity>(new Entity(2, "ball"));
	ball->cTransform = std::make_shared<CTransform>(Vec2(wWidth / 2.f, wHeight / 2.f), Vec2(4.f , 4.f)); //randomize
	ball->cShape = std::make_shared<CShape>(ballRadius, sf::Color::White, sf::Color::Black, 3.f);

	font.loadFromFile("../pixel.ttf");
	scoreText.setFont(font);
	scoreText.setCharacterSize(25);
	announceText.setFont(font);
	announceText.setCharacterSize(40);

	buffer.loadFromFile("../ball.wav");
	pongSound.setBuffer(buffer);
}

void Game::setRound() {
	player1->cInput->up = false;
	player1->cInput->down = false;
	player2->cInput->up = false;
	player2->cInput->down = false;

	player1->cTransform->pos = Vec2(playerWidth / 2 + 8.f, wHeight / 2.f);
	player1->cTransform->velocity = Vec2(0.f, 0.f);

	player2->cTransform->pos = Vec2(wWidth - playerWidth / 2 - 8.f, wHeight / 2.f);
	player2->cTransform->velocity = Vec2(0.f, 0.f);

	ball->cTransform->pos = Vec2(wWidth / 2.f, wHeight / 2.f);
	ball->cTransform->velocity = Vec2(4.f, 4.f); //randomize
}

void Game::run()
{
	while (isRunning)
	{
		if (!isPlaying)
		{
			if (!victory)
			{
				scoreText.setString("PingPong is a 2 player game");
				announceText.setString("Press space to play");
			}
			else
			{
				scoreText.setString("Press space for new game");
				if (p1Score >= maxScore) announceText.setString("Red won!");
				else if(p2Score>=maxScore)announceText.setString("Blue won!");
				p1Score = p2Score = 0;
			}
		}
		else
		{
			if (!isPaused)
			{
				player1->cTransform->velocity = player2->cTransform->velocity = { 0.f,0.f };

				if (player1->cInput->up == true) player1->cTransform->velocity.y = -7.f;
				else if (player1->cInput->down == true) player1->cTransform->velocity.y = 7.f;
	
				if (player2->cInput->up == true) player2->cTransform->velocity.y = -7.f;
				else if (player2->cInput->down == true) player2->cTransform->velocity.y = 7.f;
				
				player1->cTransform->pos += player1->cTransform->velocity;
				player2->cTransform->pos += player2->cTransform->velocity;
				ball->cTransform->pos += ball->cTransform->velocity;

				//p1 up
				if (player1->cTransform->pos.y < playerHeight / 2.f + 8.f) {
					player1->cTransform->pos.y = playerHeight / 2.f + 8.f;
				}
				//p1 dowm
				else if (player1->cTransform->pos.y > wHeight - playerHeight / 2.f - 8.f) {
					player1->cTransform->pos.y = wHeight - playerHeight / 2.f - 8.f;
				}

				//p2 up
				if (player2->cTransform->pos.y < playerHeight / 2.f + 8.f) {
					player2->cTransform->pos.y = playerHeight / 2.f + 8.f;
				}
				//p2 down
				else if (player2->cTransform->pos.y > wHeight - playerHeight / 2.f - 8.f) {
					player2->cTransform->pos.y = wHeight - playerHeight / 2.f -8.f;
				}

				//ball top
				if (ball->cTransform->pos.y < ballRadius) {
					ball->cTransform->pos.y = ballRadius + 1.f;
					ball->cTransform->velocity.y *= -1.f;
					pongSound.play();
				}
				//ball bottom
				else if (ball->cTransform->pos.y > wHeight - ballRadius) {
					ball->cTransform->pos.y = wHeight - ballRadius - 1.f;
					ball->cTransform->velocity.y *= -1.f;
					pongSound.play();
				}
				
				//ball left
				if (ball->cTransform->pos.x < ballRadius) {
					p2Score++;
					setRound();
				}
				//ball down
				else if (ball->cTransform->pos.x > wWidth - ballRadius) {
					p1Score++;
					setRound();
				}

				//if win
				if (p1Score >= maxScore || p2Score >= maxScore) {
					isPlaying = false;
					victory = true;
				}

				//hit player1
				if (ball->cTransform->pos.x - ballRadius < player1->cTransform->pos.x + playerWidth / 2.f &&
					ball->cTransform->pos.y - ballRadius >= player1->cTransform->pos.y - playerHeight / 2.f &&
					ball->cTransform->pos.y + ballRadius <= player1->cTransform->pos.y + playerHeight / 2.f)
				{
					if (ball->cTransform->pos.x - ballRadius > player1->cTransform->pos.x) 
					{
						ball->cTransform->pos.x = player1->cTransform->pos.x + playerWidth / 2.f + ballRadius + 0.1f;
						ball->cTransform->velocity.x *= -1.f;
						ball->cTransform->velocity.y += player1->cTransform->velocity.y * 0.2f;
					}
					else 
					{
						if (ball->cTransform->pos.y > player1->cTransform->pos.y) 
						{
							ball->cTransform->pos.y = player1->cTransform->pos.y + playerHeight / 2.f + ballRadius + 0.1f;
							ball->cTransform->velocity.y *= -1.f;
						}
						else
						{
							ball->cTransform->pos.y = player1->cTransform->pos.y - playerHeight / 2.f - ballRadius - 0.1f;
							ball->cTransform->velocity.y *= -1.f;
						}
					}
					pongSound.play();
				}

				//hit player2
				else if (ball->cTransform->pos.x + ballRadius > player2->cTransform->pos.x - playerWidth / 2.f &&
						 ball->cTransform->pos.y - ballRadius >= player2->cTransform->pos.y - playerHeight / 2.f &&
						 ball->cTransform->pos.y + ballRadius <= player2->cTransform->pos.y + playerHeight / 2.f)
				{
					 if (ball->cTransform->pos.x + ballRadius < player2->cTransform->pos.x) 
					 {
						 ball->cTransform->pos.x = player2->cTransform->pos.x - playerWidth / 2.f - ballRadius - 0.1f;
						 ball->cTransform->velocity.x *= -1.f;
						 ball->cTransform->velocity.y += player2->cTransform->velocity.y * 0.2f;
					 }
					 else
					 {
						 if (ball->cTransform->pos.y > player2->cTransform->pos.y)
						 {
							 ball->cTransform->pos.y = player2->cTransform->pos.y + playerHeight / 2.f + ballRadius + 0.1f;
							 ball->cTransform->velocity.y *= -1.f;
						 }
						 else
						 {
							 ball->cTransform->pos.y = player2->cTransform->pos.y - playerHeight / 2.f - ballRadius - 0.1f;
							 ball->cTransform->velocity.y *= -1.f;
						 }
					 }
					 pongSound.play();
				}

				scoreText.setString(std::to_string(p1Score)+" - "+std::to_string(p2Score));
				announceText.setString("");
			}
			else {
				announceText.setString("Paused");
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:	isRunning = false; break;

			case sf::Event::KeyPressed:
				if (isPlaying) 
				{
					switch (event.key.code)
					{
					case sf::Keyboard::W: player1->cInput->up = true; break;

					case sf::Keyboard::S: player1->cInput->down = true; break;

					case sf::Keyboard::Up: player2->cInput->up = true; break;

					case sf::Keyboard::Down: player2->cInput->down = true; break;

					case sf::Keyboard::P: isPaused = !isPaused; break;

					case sf::Keyboard::Escape: isRunning = false; break;

					default: break;
					}
				}
				else 
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Space: isPlaying = true; victory = false; break;

					case sf::Keyboard::Escape: isRunning = false; break;

					default: break;
					}
				}
				break;

			case sf::Event::KeyReleased:
				if (isPlaying) 
				{
					switch (event.key.code)
					{
					case sf::Keyboard::W: player1->cInput->up = false; break;

					case sf::Keyboard::S: player1->cInput->down = false; break;

					case sf::Keyboard::Up: player2->cInput->up = false; break;

					case sf::Keyboard::Down: player2->cInput->down = false; break;

					default: break;
					}
				}
				break;

			default: break;
			}
		}

		window.clear(sf::Color(34,187,67));

		player1->cShape->rect.setPosition(player1->cTransform->pos.x, player1->cTransform->pos.y);
		player2->cShape->rect.setPosition(player2->cTransform->pos.x, player2->cTransform->pos.y);
		ball->cShape->circle.setPosition(ball->cTransform->pos.x, ball->cTransform->pos.y);

		scoreText.setOrigin(scoreText.getLocalBounds().width / 2.f, scoreText.getLocalBounds().height / 2.f);
		announceText.setOrigin(announceText.getLocalBounds().width / 2.f, announceText.getLocalBounds().height / 2.f);

		scoreText.setPosition(wWidth / 2.f, 15.f);
		announceText.setPosition(wWidth / 2.f, wHeight / 4.f);

		window.draw(player1->cShape->rect);
		window.draw(player2->cShape->rect);
		window.draw(ball->cShape->circle);
		window.draw(scoreText);
		window.draw(announceText);

		window.display();
	}
};