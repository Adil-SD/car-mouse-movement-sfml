
#include "../include/Game.h"
#include <iostream>

// namespace Textures
// {
// 	enum ID
// 	{
// 		RedCar,
// 	};
// }

int main()
{
	try {
		Game game;
		game.run();
	} catch(std::exception& e) {
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
	// sf::RenderWindow window(sf::VideoMode(640, 480), "Resources");
	// window.setFramerateLimit(20);

	// // Try to load resources
	// ResourceHolder<sf::Texture, Textures::ID> textures;
	// try
	// {
	// 	textures.load(Textures::RedCar, "/home/adilzhan/cpp/games/cmake-sfml-project/src/red_car");
	// }
	// catch (std::runtime_error& e)
	// {
	// 	std::cout << "Exception: " << e.what() << std::endl;
	// 	return 1;
	// }
	// // Access resources
	// sf::Sprite red_car(textures.get(Textures::RedCar));
	// red_car.setScale(0.1f, 0.1f);
	// red_car.setPosition(300.f, 200.f);

	// while (window.isOpen())
	// {
	// 	sf::Event event;
	// 	while (window.pollEvent(event))
	// 	{
	// 		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed)
	// 			return 0;
	// 	}

	// 	window.clear();
	// 	window.draw(red_car);
	// 	window.display();
	// }
}