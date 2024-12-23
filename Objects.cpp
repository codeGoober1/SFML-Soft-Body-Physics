#include "Objects.hpp"

Objects::Objects(float x, float y, float pX, float pY, sf::Color color)
{
	Object.setSize(sf::Vector2f(x, y));
	Object.setFillColor(color);
	Object.setPosition(pX, pY);
	Object.setOrigin(Object.getSize() / 2.f);
	Object.setOutlineColor(sf::Color::Black);
	Object.setOutlineThickness(2);
}

void Objects::Draw(sf::RenderWindow& window)
{
	window.draw(Object);
}
