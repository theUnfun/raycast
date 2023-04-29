#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "map_geometry.h"

enum class DrawMode
{
	NONE = 0,
	Rectangle = 1,
	Line = 2
};

class Map
{
public:
	Map();

	void Update(const sf::Event& event, sf::RenderWindow& window_2d, DrawMode mode);

	void Draw2d(sf::RenderWindow& window_2d) const;

	const std::vector<ColoredLine>& GetLines() const;

	void DeleteMap();

private:
	std::vector<ColoredLine> lines_;

	bool is_drawing = false;
	sf::Vector2f start_position_;
	sf::Vector2f end_position_;


	DrawMode mode_{0};
	sf::RectangleShape temp_rectangle_;
	sf::VertexArray temp_line_ {sf::Lines, 2};
};


sf::Color GetRandomColor();