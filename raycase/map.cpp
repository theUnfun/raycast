#include "map.h"
#include <memory>
#include <iostream>

#include "constants.h"


void map::Map::Update(const sf::Event &event) {

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !is_drawing) {
    start_position_ =
        sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
		std::cout << start_position_.x << " " << start_position_.y << std::endl;
		//temp_rectangle.setPosition(start_position_);
		is_drawing = true;
	}
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && is_drawing) {
          end_position_ = sf::Vector2f((float)event.mouseButton.x,
                                       (float)event.mouseButton.y);
		std::cout << end_position_.x << " " << end_position_.y << std::endl;
		sf::Vector2f size = end_position_ - start_position_;
		Line line1 = { {start_position_.x, start_position_.y },{end_position_.x, start_position_.y} };
		Line line2 = { {end_position_.x, start_position_.y },{end_position_.x, end_position_.y} };
		Line line3 = { {end_position_.x, end_position_.y},{start_position_.x, end_position_.y} };
		Line line4 = { {start_position_.x, end_position_.y},{start_position_.x, start_position_.y } };
		lines_.emplace_back(std::move(line1));
		lines_.emplace_back(std::move(line2));
		lines_.emplace_back(std::move(line3));
		lines_.emplace_back(std::move(line4));
		//temp_rectangle.setSize(size);
		//sprites_.emplace_back(std::make_unique<sf::RectangleShape>(temp_rectangle));
		is_drawing = false;
	}
	/*else if (is_drawing) {

	}*/
}


std::vector<sf::Vector2f> map::Map::GetCollisionsForRay(const Line& ray) const {
	std::vector<sf::Vector2f> result;
	for (const auto& line : lines_) {
		auto collision = GetCollision(line, ray);
		if (collision.has_value()) {
			result.emplace_back(collision.value());
		}
	}
	return result;
}