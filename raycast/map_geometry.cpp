#include "map_geometry.h"


std::optional<sf::Vector2f> GetIntersection(const Line& line1, const Line& line2)
{
	float a1 = line1.p2.y - line1.p1.y;
	float b1 = line1.p1.x - line1.p2.x;
	float c1 = line1.p2.x * line1.p1.y - line1.p1.x * line1.p2.y;

	float a2 = line2.p2.y - line2.p1.y;
	float b2 = line2.p1.x - line2.p2.x;
	float c2 = line2.p2.x * line2.p1.y - line2.p1.x * line2.p2.y;

	float det = a1 * b2 - a2 * b1;

	if (det == 0)
	{
		// parallel
		return std::nullopt;
	}
	else
	{
		float x = (b1 * c2 - b2 * c1) / det;
		float y = (a2 * c1 - a1 * c2) / det;
		sf::Vector2f intersection(x, y);

		if (std::abs(line1.p1.x - line1.p2.x) > EPSILON)
		{
			float t1 = (intersection.x - line1.p1.x) / (line1.p2.x - line1.p1.x);
			if (t1 < 0 || t1 > 1)
			{
				return std::nullopt;
			}
		}
		else
		{
			float t1 = (intersection.y - line1.p1.y) / (line1.p2.y - line1.p1.y);
			if (t1 < 0 || t1 > 1)
			{
				return std::nullopt;
			}
		}

		if (std::abs(line2.p1.x - line2.p2.x) > EPSILON)
		{
			float t2 = (intersection.x - line2.p1.x) / (line2.p2.x - line2.p1.x);
			if (t2 < 0 || t2 > 1)
			{
				return std::nullopt;
			}
		}
		else
		{
			float t2 = (intersection.y - line2.p1.y) / (line2.p2.y - line2.p1.y);
			if (t2 < 0 || t2 > 1)
			{
				return std::nullopt;
			}
		}

		return intersection;
	}
}

float GetDistanceBetweenObjects(const sf::Vector2f& position1, const sf::Vector2f& position2)
{
	return std::sqrt(std::pow(position2.x - position1.x, 2.0f) + std::pow(position2.y - position1.y, 2.0f));
}