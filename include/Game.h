#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

class Game
{
    public:
        						Game();
		void					run();

    private:
        void					processEvents();
		void					update(sf::Time elapsedTime);
		void					render();

		void					updateStatistics(sf::Time elapsedTime);	
		void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

		void 					carMoving(sf::Vector2f* movement);
		
    
    private:
        static const float		CarSpeed;
		static const float		CarWheelRotationSpeed;

		sf::RenderWindow		mWindow;
		sf::Texture				mTexture;
		sf::Sprite				mCar;
		sf::Font				mFont;
		sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;

		std::size_t				mStatisticsNumFrames;
		bool					mIsMovingUp;
		bool					mIsMovingDown;
		bool					mIsMovingRight;
		bool					mIsMovingLeft;
		float					mDirectionAngle;

};