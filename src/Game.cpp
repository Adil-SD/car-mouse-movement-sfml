#include "../include/Game.h"
#include "../include/StringHelpers.h"
#include <cmath>

const float Game::CarSpeed = 150.f;
const float Game::CarWheelRotationSpeed = 0.1f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: mWindow(sf::VideoMode(640, 480), "Car simulation", sf::Style::Close)
, mTexture()
, mCar()
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mIsMovingUp(false)
, mIsMovingDown(false)
, mIsMovingRight(false)
, mIsMovingLeft(false)
, mDirectionAngle(0.f)
{
	if (!mTexture.loadFromFile("/home/adilzhan/cpp/games/cmake-sfml-project/src/red_car"))
	{
		// Handle Error
	}

	mCar.setTexture(mTexture);
    mCar.setScale(0.1f, 0.1f);
	mCar.setPosition(100.f, 100.f);
	mCar.setOrigin(mCar.getLocalBounds().width / 2, mCar.getLocalBounds().height / 2);
	
	mFont.loadFromFile("/home/adilzhan/cpp/games/cmake-sfml-project/src/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;

			case sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;

			case sf::Event::Closed:
				mWindow.close();
				break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);

	// ********************
	// MOVING WITH WASD
	// ********************


	if (mIsMovingUp && !mIsMovingLeft && !mIsMovingRight && !mIsMovingDown)
	{
		carMoving(&movement);
	}
	if (mIsMovingDown && !mIsMovingLeft && !mIsMovingRight && !mIsMovingUp)
	{
		mCar.rotate(180.f);
		carMoving(&movement);
		mCar.rotate(180.f);
	}
	if (mIsMovingUp && mIsMovingLeft && !mIsMovingRight && !mIsMovingDown)
	{
		mCar.rotate(-1.f);
		carMoving(&movement);
	}
	if (mIsMovingUp && !mIsMovingLeft && mIsMovingRight && !mIsMovingDown)
	{
		mCar.rotate(1.f);
		carMoving(&movement);
	}
	if (mIsMovingDown && mIsMovingLeft && !mIsMovingRight && !mIsMovingUp)
	{
		mCar.rotate(-179.f);
		carMoving(&movement);
		mCar.rotate(180.f);
	}
	if (mIsMovingDown && !mIsMovingLeft && mIsMovingRight && !mIsMovingUp)
	{
		mCar.rotate(179.f);
		carMoving(&movement);
		mCar.rotate(180.f);
	}

	// ************************
	// MOVING WITH MOUSE
	// ************************
	
		
	
	mCar.move(movement * elapsedTime.asSeconds());
}

void Game::carMoving(sf::Vector2f* movement)
{
	if(mCar.getRotation() > 0.0f && mCar.getRotation() <= 90.0f) {
		(*movement).x += (std::sin(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
		(*movement).y -= (std::cos(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
	}
	else if(mCar.getRotation() > 90.0f && mCar.getRotation() <= 180.0f) {
		(*movement).x += (std::sin(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
		(*movement).y += -(std::cos(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
	}
	else if(mCar.getRotation() > 180.0f && mCar.getRotation() <= 270.0f) {
		(*movement).x -= -(std::sin(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
		(*movement).y += -(std::cos(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
	} else {
		(*movement).x -= -(std::sin(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
		(*movement).y -= (std::cos(mCar.getRotation() * (M_PI / 180.0f)) * CarSpeed);
	}
}

void Game::render()
{
	mWindow.clear();	
	mWindow.draw(mCar);
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
							 
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{	
	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}