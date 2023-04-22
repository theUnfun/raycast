#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


// Forwards.
class Map;


class Player
{
public:

	Player(int x, int y, const Map& map);

	void Update(float deltatime);

	void Draw(sf::RenderWindow& window_2d, sf::RenderWindow& window_3d) const;


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


};

