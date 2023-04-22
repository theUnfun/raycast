#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


namespace map {
	class Map;
}


class Player
{
public:
	Player(int x, int y, const map::Map& map);

	void Update(float deltatime);

	void Draw(sf::RenderWindow& window_2d, sf::RenderWindow& window_3d);


private:
	float GetDistanceBetweenObjects(const sf::Vector2f& position1, const sf::Vector2f& position2);

	void DrawPlayer(sf::RenderWindow& window_2d) const;

	void DrawVisionRay(sf::RenderWindow& window_2d) const;

	void Drawbackground(sf::RenderWindow& window_3d) const;

	void ComputeDistances();

	void MovePlayer(float deltatime);

private:
	sf::Vector2f position_;

	float direction_ = 40.;  // angle
	std::vector<float> distances_;

	std::vector<sf::VertexArray> rays_;
	std::vector<sf::Vector2f> collis;

	float projection_distance_;

	const map::Map& map_;
	sf::CircleShape texture_;
};