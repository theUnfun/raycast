#include "player.h"
#include "map.h"
#include "constants.h"
#include <iostream>
#include <cmath>


Player::Player(int x, int y, const map::Map& map)
    : position_((float)x, (float)y)
    , projection_distance_(0.5f * (float)WALL_HEIGHT / std::tan(pi / 180.0f * (0.5f * FOV_VERTICAL)))
    , map_(map)
{
	rays_.resize(NUMBER_OF_RAYS);
	distances_.resize(NUMBER_OF_RAYS);
}

void Player::Update(float deltatime)
{
	MovePlayer(deltatime);
	ComputeDistances();
}

void Player::Draw(sf::RenderWindow& window_2d, sf::RenderWindow& window_3d)
{
	DrawVisionRay(window_2d);
	Drawbackground(window_3d);


	float i = 0;


	for (const auto& distance : distances_)
	{
		float projection_distance = 0.5f * WALL_HEIGHT / std::tan(pi / 180 * (0.5f * FOV_VERTICAL));
		float height = ((float)WINDOW_HEIGHT * projection_distance / distance);
		// float max_height = (WINDOW_HEIGHT * projection_distance / VISION_RANGE);

		// unsigned char fog_brightness = static_cast<unsigned char>(255 * (1 - distance / VISION_RANGE));

		/*sf::RectangleShape fog_rect(sf::Vector2f(WINDOW_WIDTH / FOV_HORIZONTAL, max_height));
		fog_rect.setFillColor(sf::Color(100, 100, 100));
		fog_rect.setOrigin(0, -WINDOW_HEIGHT / 2 + height / 2);
		fog_rect.setPosition(i, 0);
		window_3d.draw(fog_rect);*/
		if (std::abs(distance - VISION_RANGE) > EPSILON)
		{
			sf::RectangleShape rect(sf::Vector2f(WINDOW_WIDTH / FOV_HORIZONTAL, height));
			rect.setFillColor(sf::Color((uint8_t)(255.0f * (1 - distance / VISION_RANGE)),
			                            (uint8_t)(255.0f * (1 - distance / VISION_RANGE)),
			                            (uint8_t)(255.0f * (1 - distance / VISION_RANGE)) /*, brightness*/));
			rect.setOrigin(0, -(float)WINDOW_HEIGHT / 2.0f + height / 2.0f);
			rect.setPosition(i, 0);
			window_3d.draw(rect);
		}

		i += WINDOW_WIDTH / NUMBER_OF_RAYS;
	}

	for (const auto& ray : rays_)
	{
		window_2d.draw(ray);
	}

	for (const auto& col : collis)
	{
		sf::CircleShape circle(3);
		circle.setOrigin(2.5, 2.5);
		circle.setPosition(col);
		circle.setFillColor(sf::Color::Red);
		window_2d.draw(circle);
	}

	DrawPlayer(window_2d);
}

float Player::GetDistanceBetweenObjects(const sf::Vector2f& position1, const sf::Vector2f& position2)
{
	return std::sqrt(std::pow(position2.x - position1.x, 2.0f) + std::pow(position2.y - position1.y, 2.0f));
}

void Player::DrawPlayer(sf::RenderWindow& window_2d) const
{
	sf::CircleShape player(20);
	player.setOrigin(20, 20);
	player.setPosition(position_);
	window_2d.draw(player);
}

void Player::DrawVisionRay(sf::RenderWindow& window_2d) const
{
	sf::VertexArray vision_ray(sf::Lines, 2);
	vision_ray[0].position = sf::Vector2f(position_);
	vision_ray[1].position = sf::Vector2f(position_.x - VISION_RANGE * std::cos(pi / 180.0f * (direction_ + 90.0f)),
	                                      position_.y - VISION_RANGE * std::sin(pi / 180.0f * (direction_ + 90.0f)));
	vision_ray[0].color = sf::Color::Red;
	vision_ray[1].color = sf::Color::Blue;
	window_2d.draw(vision_ray);
}

void Player::Drawbackground(sf::RenderWindow& window_3d) const
{
	sf::RectangleShape ground({WINDOW_WIDTH, WINDOW_HEIGHT / 2});
	ground.setFillColor(sf::Color(188, 93, 88));
	ground.setPosition(0, WINDOW_HEIGHT / 2);
	window_3d.draw(ground);
	sf::RectangleShape sky({WINDOW_WIDTH, WINDOW_HEIGHT / 2});
	sky.setFillColor(sf::Color(142, 180, 212));
	sky.setPosition(0, 0);
	window_3d.draw(sky);
}

void Player::ComputeDistances()
{
	rays_.clear();
	distances_.clear();
	collis.clear();
	/*const auto bounds = map_.GetAllGlobalBounds();*/

	float half_far_plane_width = VISION_RANGE * std::tan((pi / 180) * FOV_HORIZONTAL / 2);
	for (int i = 0; i < NUMBER_OF_RAYS; ++i)
	{
		float pixel_pos = half_far_plane_width * 2.0f * ((float)i + 0.5f - NUMBER_OF_RAYS / 2.0f) / (float)NUMBER_OF_RAYS;

		float angle = std::atan(pixel_pos / VISION_RANGE) * 180 / pi;
		float max_ray_length = std::sqrt(pixel_pos * pixel_pos + VISION_RANGE * VISION_RANGE);

		sf::VertexArray ray(sf::Lines, 2);
		ray[0].position = sf::Vector2f(position_);
		ray[1].position = sf::Vector2f(position_.x - max_ray_length * std::cos(pi / 180.0f * (angle + direction_ + 90.0f)),
		                               position_.y - max_ray_length * std::sin(pi / 180.0f * (angle + direction_ + 90.0f)));

		map::Line ray_line{ray[0].position, ray[1].position};

		auto collisions = map_.GetCollisionsForRay(ray_line);
		/*bool found = false;
		for (float ray_length = 0; ray_length < max_ray_length; ray_length += 0.5) {
		    if (found == true) {
		        break;
		    }
		    float x = position_.x - ray_length * cos(pi / 180 * (angle + direction_ + 90.));
		    float y = position_.y - ray_length * sin(pi / 180 * (angle + direction_ + 90.));
		    for (const auto& bound : bounds)
		        if (bound.contains({ x,y })) {
		            ray[1].position = sf::Vector2f(x, y);
		            found = true;
		            break;
		        }
		}*/
		for (const auto& col : collisions)
		{
			float distance = GetDistanceBetweenObjects(ray[0].position, col);
			float depth = distance * cos(pi / 180 * (angle));

			collis.emplace_back(col);
			distances_.emplace_back(depth);
			// rays_.emplace_back(std::move(ray));
		}
	}
}


void Player::MovePlayer(float deltatime)
{
	sf::Vector2f step = {0, 0};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		step.x += -MOVEMENT_SPEED * std::cos(pi / 180 * direction_);
		step.y += -MOVEMENT_SPEED * std::sin(pi / 180 * direction_);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		step.x += MOVEMENT_SPEED * std::cos(pi / 180 * direction_);
		step.y += MOVEMENT_SPEED * std::sin(pi / 180 * direction_);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		step.x += MOVEMENT_SPEED * cos(pi / 180.0f * (direction_ + 90.0f));
		step.y += MOVEMENT_SPEED * std::sin(pi / 180.0f * (direction_ + 90.0f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		step.x += -MOVEMENT_SPEED * std::cos(pi / 180.0f * (direction_ + 90.0f));
		step.y += -MOVEMENT_SPEED * std::sin(pi / 180.0f * (direction_ + 90.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		direction_ -= ROTATION_SPEED * deltatime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		direction_ += ROTATION_SPEED * deltatime;
		;
	}
	position_ += step * deltatime;
}