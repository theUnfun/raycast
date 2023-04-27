#include "map.h"
#include <memory>
#include <iostream>

#include "constants.h"

Map::Map() {
	temp_rectangle_.setFillColor(sf::Color::Green);
}

void Map::Update(const sf::Event& event, sf::RenderWindow& window_2d)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !is_drawing)
	{
		start_position_ = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		std::cout << start_position_.x << " " << start_position_.y << std::endl;
		is_drawing = true;
	}
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && is_drawing)
	{
		end_position_ = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		std::cout << end_position_.x << " " << end_position_.y << std::endl;

		lines_.emplace_back(ColoredLine{sf::Vector2f(start_position_.x, start_position_.y),
		                                sf::Vector2f(end_position_.x, start_position_.y), GetRandomColor()});
		lines_.emplace_back(ColoredLine{sf::Vector2f(end_position_.x, start_position_.y),
		                                sf::Vector2f(end_position_.x, end_position_.y), GetRandomColor()});
		lines_.emplace_back(ColoredLine{sf::Vector2f(end_position_.x, end_position_.y),
		                                sf::Vector2f(start_position_.x, end_position_.y), GetRandomColor()});
		lines_.emplace_back(ColoredLine{sf::Vector2f(start_position_.x, end_position_.y),
		                                sf::Vector2f(start_position_.x, start_position_.y), GetRandomColor()});

		is_drawing = false;
	}
	else if (is_drawing)
	{
		end_position_ = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window_2d));
		temp_rectangle_.setPosition(start_position_);
		temp_rectangle_.setSize(end_position_ - start_position_);
	}
}

void Map::Draw2d(sf::RenderWindow& window_2d) const
{
	for (const auto& line : lines_)
	{
		sf::VertexArray line_to_draw(sf::Lines, 2);
		line_to_draw[0].position = line.p1;
		line_to_draw[1].position = line.p2;
		window_2d.draw(line_to_draw);
	}
	if (is_drawing)
	{
		window_2d.draw(temp_rectangle_);
	}
}

const std::vector<ColoredLine>& Map::GetLines() const
{
	return lines_;
}

sf::Color GetRandomColor()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	int r = std::rand() % 256;
	int g = std::rand() % 256;
	int b = std::rand() % 256;

	return sf::Color(r, g, b);
}