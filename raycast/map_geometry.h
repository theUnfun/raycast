#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "constants.h"


struct Line
{
	sf::Vector2f p1 = {0, 0};
	sf::Vector2f p2 = {0, 0};
};

struct WallLine : Line
{
	sf::Color color = {0, 0, 0};
};


std::optional<sf::Vector2f> GetIntersection(const Line& line1, const Line& line2);

float GetDistanceBetweenObjects(const sf::Vector2f& position1, const sf::Vector2f& position2);