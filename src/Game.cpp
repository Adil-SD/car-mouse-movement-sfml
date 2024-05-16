#include "../include/Game.h"
#include "../include/StringHelpers.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

const float Game::CarAcceleration = 50.0f;
const float Game::CarMaxSpeed = 300.0f;
const float Game::CarMaxRotationAngle = 60.0f;

double cot(double angle)
{
	return 1.0 / std::tan(angle);
}

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "Car simulation", sf::Style::Close), mTexture(), mCar(), mFont(), mStatisticsText(), mStatisticsUpdateTime(), mStatisticsNumFrames(0), mIsMovingUp(false), mIsMovingDown(false), mIsMovingRight(false), mIsMovingLeft(false), CarCurrentSpeed(0.f)
{
	if (!mTexture.loadFromFile("../assets/red_car"))
	{
		// Handle Error
	}

	mCar.setTexture(mTexture);
	mCar.setScale(0.1f, 0.1f);
	mCar.setPosition(200.f, 200.f);
	mCar.setOrigin(mCar.getLocalBounds().width / 2, mCar.getLocalBounds().height / 2);

	mFont.loadFromFile("../assets/Sansation.ttf");
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

float vectorMagnitude(const sf::Vector2f &v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

float angleBetweenVectors(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
	float dotProduct = v1.x * v2.x + v1.y * v2.y;
	float magnitudeV1 = vectorMagnitude(v1);
	float magnitudeV2 = vectorMagnitude(v2);
	return std::acos(dotProduct / (magnitudeV1 * (magnitudeV2 + 0.000001)	)) * 180.0f / M_PI; // Convert to degrees
}

int sideOfVector(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
	float crossProduct = v1.x * v2.y - v1.y * v2.x;

	if (crossProduct >= 0)
	{
		return 1; // Second vector is on one side
	}
	else
	{
		return -1; // Second vector is on the other side
	}
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsAccelerating)
		CarCurrentSpeed += CarAcceleration * elapsedTime.asSeconds();
	
	if (CarCurrentSpeed > CarMaxSpeed)
	{
		CarCurrentSpeed = CarMaxSpeed;
	}
	else
	{
		CarCurrentSpeed -= 20.0 * elapsedTime.asSeconds();

		if (CarCurrentSpeed < 0.0)
		{
			CarCurrentSpeed = 0.0;
		}
	}

	if (CarCurrentSpeed > 0)
	{
		// Direction is a vector starting from car origin to mouse position
		sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
		sf::Vector2f direction = static_cast<sf::Vector2f>(mousePos) - mCar.getPosition();

		// carDirection is the vector showing the direction where car is looking
		float x = sin(mCar.getRotation() * M_PI / 180.0);
		float y = -cos(mCar.getRotation() * M_PI / 180.0);
		sf::Vector2f carDirection(x, y);

		float angle = angleBetweenVectors(carDirection, direction);

		int side = sideOfVector(carDirection, direction);
		// std::cout << angle << std::endl;
		if (angle != 0)
		{
			if (angle > CarMaxRotationAngle * elapsedTime.asSeconds())
			{
				mCar.rotate(CarMaxRotationAngle * side * elapsedTime.asSeconds());
			}
			else
			{
				mCar.rotate(angle * side * elapsedTime.asSeconds());
			}
		}
		carMoving(&movement);
	}

	mCar.move(movement * elapsedTime.asSeconds());
}

void Game::carMoving(sf::Vector2f *movement)
{
	float sinDirection = (std::sin(mCar.getRotation() * (M_PI / 180.0f)) * CarCurrentSpeed);
	float cosDirection = (std::cos(mCar.getRotation() * (M_PI / 180.0f)) * CarCurrentSpeed);

	if (mCar.getRotation() > 0.0f && mCar.getRotation() <= 90.0f)
	{
		(*movement).x += sinDirection;
		(*movement).y -= cosDirection;
	}
	else if (mCar.getRotation() > 90.0f && mCar.getRotation() <= 180.0f)
	{
		(*movement).x += sinDirection;
		(*movement).y += -cosDirection;
	}
	else if (mCar.getRotation() > 180.0f && mCar.getRotation() <= 270.0f)
	{
		(*movement).x -= -sinDirection;
		(*movement).y += -cosDirection;
	}
	else
	{
		(*movement).x -= -sinDirection;
		(*movement).y -= cosDirection;
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
	else if (key == sf::Keyboard::Space)
		mIsAccelerating = isPressed;
}