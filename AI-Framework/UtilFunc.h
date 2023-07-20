#pragma once
#include <SFML/Graphics.hpp>

inline float Distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
	sf::Vector2f c = a - b;

	return std::sqrtf(c.x * c.x + c.y * c.y);
}

inline float Magnitude(const sf::Vector2f& a)
{
	return std::sqrtf(a.x * a.x + a.y * a.y);
}

inline bool CloseEnough(const sf::Vector2f& position1, const sf::Vector2f& position2, float tolerance)
{
	return abs(Distance(position1, position2)) <= tolerance;
}