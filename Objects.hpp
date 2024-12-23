#pragma once
#include <SFML/Graphics.hpp>

class Objects
{
public:
	Objects(float x, float y, float pX, float pY, sf::Color color);
	void Draw(sf::RenderWindow& window);

	sf::Vector2f GetPosition()
	{
		return Object.getPosition();
	}

	sf::Vector2f GetSize()
	{
		return Object.getSize() / 2.f;
	}

	void Move(float dx, float dy)
	{
		Object.move(dx, dy);
	}

private:
	sf::RectangleShape Object;
};
