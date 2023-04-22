#include <SFML/Graphics.hpp>
#include "player.h"
#include "map.h"
#include "constants.h"


int main()
{
	sf::RenderWindow window_2d(sf::VideoMode(700, 700), "2d");
	sf::RenderWindow window_3d(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3d");
	window_2d.setFramerateLimit(60);
	sf::Clock clock;

	map::Map m;
	Player p(50, 600, m);
	float deltaTime = 0.f;

	while (window_2d.isOpen())
	{
		sf::Event event;
		while (window_2d.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window_2d.close();
		}
		deltaTime = clock.restart().asSeconds();

		p.Update(deltaTime);
		m.Update(event);

		window_2d.clear();
		window_3d.clear();
		//m.Draw2d(window_2d);
		p.Draw(window_2d,window_3d);

		window_2d.display();
		window_3d.display();
	}
}