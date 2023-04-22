#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "constants.h"

namespace map {

	struct Line {
		sf::Vector2f p1 = { 0,0 };
		sf::Vector2f p2 = { 0,0 };
	};

	struct WallLine : Line {
		sf::Color color = { 0,0,0 };
	};

	std::optional<sf::Vector2f> GetCollision(const Line& line1, const Line& line2);

} //namespace map