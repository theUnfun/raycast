#include "camera.h"
#include "map.h"
#include "constants.h"
#include <iostream>
#include <cmath>


Camera::Camera(int x, int y, const Map& map)
    : position_(static_cast<float>(x), static_cast<float>(y))
    , projection_distance_(0.5f * static_cast<float>(WALL_HEIGHT) / std::tan(pi / 180.0f * (0.5f * FOV_VERTICAL)))
    , map_(map)
{
	rays_.resize(MAX_NUMBER_OF_RAYS);
	distances_.resize(MAX_NUMBER_OF_RAYS);
	colors_.resize(MAX_NUMBER_OF_RAYS);
}


void Camera::Update(float deltatime)
{
	MovePlayer(deltatime);
	ComputeDistances();
}


void Camera::Draw(sf::RenderWindow& window_2d, sf::RenderWindow& window_3d) const
{
	DrawVisionRay(window_2d);
	DrawBackground(window_3d);


	float screen_pos = 0;

	for (size_t i = 0; i < distances_.size(); i++)
	{
		float projection_distance = 0.5f * WALL_HEIGHT / std::tan(pi / 180 * (0.5f * FOV_VERTICAL));
		float height = (static_cast<float>(WINDOW_HEIGHT) * projection_distance / distances_[i]);

		if (std::abs(distances_[i] - VISION_RANGE) > EPSILON)
		{
			sf::RectangleShape rect(sf::Vector2f(static_cast<float>(WINDOW_WIDTH) / NUMBER_OF_RAYS, height));

			rect.setFillColor(sf::Color(static_cast<uint8_t>(colors_[i].r * (1 - distances_[i] / VISION_RANGE)),
			                            static_cast<uint8_t>(colors_[i].g * (1 - distances_[i] / VISION_RANGE)),
			                            static_cast<uint8_t>(colors_[i].b * (1 - distances_[i] / VISION_RANGE))));
			rect.setOrigin(0, -static_cast<float>(WINDOW_HEIGHT) / 2.0f + height / 2.0f);
			rect.setPosition(screen_pos, 0);
			window_3d.draw(rect);
		}

		screen_pos += static_cast<float>(WINDOW_WIDTH) / NUMBER_OF_RAYS;
	}

	for (const auto& ray : rays_)
	{
		window_2d.draw(ray);
	}

	for (const auto& col : intersections_)
	{
		sf::CircleShape circle(3);
		circle.setOrigin(2.5, 2.5);
		circle.setPosition(col);
		circle.setFillColor(sf::Color::Red);
		window_2d.draw(circle);
	}

	DrawPlayer(window_2d);
}

void Camera::DrawPlayer(sf::RenderWindow& window_2d) const
{
	sf::CircleShape player(20);
	player.setOrigin(20, 20);
	player.setPosition(position_);
	window_2d.draw(player);
}

void Camera::DrawVisionRay(sf::RenderWindow& window_2d) const
{
	sf::VertexArray vision_ray(sf::Lines, 2);
	vision_ray[0].position = sf::Vector2f(position_);
	vision_ray[1].position = sf::Vector2f(position_.x - VISION_RANGE * std::cos(pi / 180.0f * (direction_ + 90.0f)),
	                                      position_.y - VISION_RANGE * std::sin(pi / 180.0f * (direction_ + 90.0f)));
	vision_ray[0].color = sf::Color::Red;
	vision_ray[1].color = sf::Color::Blue;
	window_2d.draw(vision_ray);
}

void Camera::DrawBackground(sf::RenderWindow& window_3d) const
{
	sf::RectangleShape ground({WINDOW_WIDTH, WINDOW_HEIGHT / 2});
	ground.setFillColor(ground_color);
	ground.setPosition(0, static_cast<float>(WINDOW_HEIGHT) / 2);
	window_3d.draw(ground);
	sf::RectangleShape sky({WINDOW_WIDTH, WINDOW_HEIGHT / 2});
	sky.setFillColor(sky_color);
	sky.setPosition(0, 0);
	window_3d.draw(sky);
}

void Camera::ComputeDistances()
{
	rays_.resize(NUMBER_OF_RAYS);
	distances_.resize(NUMBER_OF_RAYS);
	intersections_.resize(NUMBER_OF_RAYS);

	rays_.clear();
	distances_.clear();
	intersections_.clear();

	float half_far_plane_width = VISION_RANGE * std::tan((pi / 180) * FOV_HORIZONTAL / 2);
	for (uint32_t i = 0; i < NUMBER_OF_RAYS; ++i)
	{
		float pixel_pos =
		    half_far_plane_width * 2.0f * (static_cast<float>(i) + 0.5f - NUMBER_OF_RAYS / 2.0f) / NUMBER_OF_RAYS;

		float angle = std::atan(pixel_pos / VISION_RANGE) * 180 / pi;
		float max_ray_length = std::sqrt(pixel_pos * pixel_pos + VISION_RANGE * VISION_RANGE);

		sf::VertexArray ray(sf::Lines, 2);
		ray[0].position = sf::Vector2f(position_);
		ray[1].position = sf::Vector2f(position_.x - max_ray_length * std::cos(pi / 180.0f * (angle + direction_ + 90.0f)),
		                               position_.y - max_ray_length * std::sin(pi / 180.0f * (angle + direction_ + 90.0f)));


		intersections_.emplace_back(0.0f, 0.0f);
		distances_.emplace_back(VISION_RANGE);
		for (const auto& line : map_.GetLines())
		{
			auto collision = GetIntersection(line, Line{ray[0].position, ray[1].position});
			if (collision.has_value())
			{
				float current_depth = distances_.back();

				float distance = GetDistanceBetweenObjects(ray[0].position, collision.value());
				float depth = distance * cos(pi / 180 * (angle));

				if (current_depth > depth)
				{
					distances_.back() = depth;
					intersections_.back() = collision.value();
					colors_[i] = line.color;
				}
			}
		}

		rays_.emplace_back(std::move(ray));
	}
}


void Camera::MovePlayer(float deltatime)
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
	}
	position_ += step * deltatime;
}


// Getters and setters for variables
/////////////
void Camera::SetNumberOfRays(int num)
{
	NUMBER_OF_RAYS = num;
}

uint32_t Camera::GetNumberOfRays()
{
	return NUMBER_OF_RAYS;
}

void Camera::SetHorizotalFOV(float num)
{
	FOV_HORIZONTAL = num;
}

float Camera::GetHorizotalFOV()
{
	return FOV_HORIZONTAL;
}

void Camera::SetVerticalFOV(float num)
{
	FOV_VERTICAL = num;
}

float Camera::GetVerticalFOV()
{
	return FOV_VERTICAL;
}

void Camera::SetVisionRange(float num)
{
	VISION_RANGE = num;
}

float Camera::GetVisionRange()
{
	return VISION_RANGE;
}

void Camera::SetSkyColor(uint8_t r, uint8_t g, uint8_t b)
{
	sky_color = {r, g, b};
}
sf::Color Camera::GetSkyColor()
{
	return sky_color;
}

void Camera::SetGroundColor(uint8_t r, uint8_t g, uint8_t b)
{
	ground_color = {r, g, b};
}
sf::Color Camera::GetGroundColor()
{
	return ground_color;
}

/////////////