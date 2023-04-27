#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "map_geometry.h"


class Map
{
public:

	Map();

	void Update(const sf::Event& event, sf::RenderWindow& window_2d);

	void Draw2d(sf::RenderWindow& window_2d) const;

	const std::vector<ColoredLine>& GetLines() const;

private:

	std::vector<ColoredLine> lines_;

	bool is_drawing = false;
	sf::Vector2f start_position_;
	sf::Vector2f end_position_;

	sf::RectangleShape temp_rectangle_;
};


sf::Color GetRandomColor();