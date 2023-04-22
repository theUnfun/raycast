#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "map_geometry.h"


class Map
{
public:
	void Update(const sf::Event& event);


	// void Draw2d(sf::RenderWindow& window_2d) const {

	//	for (const auto& sprite : sprites_) {
	//		window_2d.draw(*sprite);
	//	}
	//}


	std::vector<sf::Vector2f> GetCollisionsForRay(const Line& ray) const;

	//__declspec(noinline) auto GetAllGlobalBounds() const {
	//	if (sprites_.empty()) {
	//		return std::vector<sf::FloatRect>();
	//	}
	//	std::vector<sf::FloatRect> result;
	//	result.reserve(sprites_.size());
	//	for (const auto& sprite : sprites_)
	//		result.emplace_back(sprite->getGlobalBounds());
	//	return result;
	//}

private:
	std::vector<Line> lines_;
	bool is_drawing = false;
	sf::Vector2f start_position_;
	sf::Vector2f end_position_;
};
