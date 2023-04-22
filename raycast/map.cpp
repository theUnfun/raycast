#include "map.h"
#include <memory>
#include <iostream>

#include "constants.h"


void Map::Update(const sf::Event& event, sf::RenderWindow& window_2d)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !is_drawing)
	{
		start_position_ = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
		std::cout << start_position_.x << " " << start_position_.y << std::endl;
		is_drawing = true;
	}
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && is_drawing)
	{
		end_position_ = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
		std::cout << end_position_.x << " " << end_position_.y << std::endl;
		lines_.emplace_back(Line{{start_position_.x, start_position_.y}, {end_position_.x, start_position_.y}});
		lines_.emplace_back(Line{{end_position_.x, start_position_.y}, {end_position_.x, end_position_.y}});
		lines_.emplace_back(Line{{end_position_.x, end_position_.y}, {start_position_.x, end_position_.y}});
		lines_.emplace_back(Line{{start_position_.x, end_position_.y}, {start_position_.x, start_position_.y}});
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


std::vector<sf::Vector2f> Map::GetIntersectionsForRay(const Line& ray) const
{
	std::vector<sf::Vector2f> result;
	for (const auto& line : lines_)
	{
		auto collision = GetIntersection(line, ray);
		if (collision.has_value())
		{
			result.emplace_back(collision.value());
		}
	}

	return result;
}