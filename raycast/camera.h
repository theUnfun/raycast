#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "constants.h"


// Forwards.
class Map;

class Camera
{
public:
	Camera(int x, int y, const Map& map);

	void Update(float deltatime);

	void Draw(sf::RenderWindow& window_2d, sf::RenderWindow& window_3d) const;


	// Getters and setters for variables
	void SetNumberOfRays(int);
	uint32_t GetNumberOfRays();
	void SetHorizotalFOV(float);
	float GetHorizotalFOV();
	void SetVerticalFOV(float);
	float GetVerticalFOV();
	void SetVisionRange(float);
	float GetVisionRange();
	void SetSkyColor(uint8_t, uint8_t, uint8_t);
	sf::Color GetSkyColor();
	void SetGroundColor(uint8_t, uint8_t, uint8_t);
	sf::Color GetGroundColor();


private:
	void DrawPlayer(sf::RenderWindow& window_2d) const;

	void DrawVisionRay(sf::RenderWindow& window_2d) const;

	void DrawBackground(sf::RenderWindow& window_3d) const;

	void ComputeDistances();

	void MovePlayer(float deltatime);

private:
	const Map& map_;
	sf::Vector2f position_;
	float projection_distance_;
	float direction_ = 40.;  // angle

	std::vector<float> distances_;
	std::vector<sf::VertexArray> rays_;
	std::vector<sf::Vector2f> intersections_;
	std::vector<sf::Color> colors_;

	// variables
	uint32_t NUMBER_OF_RAYS = 1200;
	float FOV_HORIZONTAL = 80.0f;
	float FOV_VERTICAL = 60.0f;
	float VISION_RANGE = 500.;
	sf::Color sky_color = {142, 180, 212};
	sf::Color ground_color = {188, 93, 88};
};
