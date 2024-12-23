#include "SoftBody.hpp"
#include <iostream>
#include <algorithm>


SoftBody::SoftBody(float Restitution, float SpringConstant, float Damping) :
	Restitution(Restitution), SpringConstant(SpringConstant), Damping(Damping) 
{
	ResText.setFillColor(sf::Color::Red);
	ResText.setCharacterSize(15);
	ResText.setPosition(100, 50);
	ResText.setString(std::to_string(this->Restitution));

	ResTitle.setFillColor(sf::Color::Black);
	ResTitle.setCharacterSize(15);
	ResTitle.setPosition(92, 30);
	ResTitle.setString("Restitution:");

	SpringConstText.setFillColor(sf::Color::Red);
	SpringConstText.setCharacterSize(15);
	SpringConstText.setPosition(100, 120);
	SpringConstText.setString(std::to_string(this->SpringConstant));

	SpringConstTitle.setFillColor(sf::Color::Black);
	SpringConstTitle.setCharacterSize(15);
	SpringConstTitle.setPosition(92, 100);
	SpringConstTitle.setString("SpringConstant:");

	button1.setFillColor(sf::Color::White);
	button1.setSize(sf::Vector2f(50, 50));

	button2.setFillColor(sf::Color::White);
	button2.setSize(sf::Vector2f(50, 50));

	button3.setFillColor(sf::Color::White);
	button3.setSize(sf::Vector2f(50, 50));

	button4.setFillColor(sf::Color::White);
	button4.setSize(sf::Vector2f(50, 50));
}

void SoftBody::AddPoint(float x, float y)
{
	sf::Vector2f vertex = { x, y };
	softBody.push_back(vertex);
	// We add a point to vertex (x, y)

	sf::Vector2f zero = { 0, 0 };
	velocity.push_back(zero);
	force.push_back(zero);
	// Right now we set velocity and force to 0 until we acclerate the points.

	for (int i = 0; i < softBody.size(); i++)
	{
		for (int j = 0; j < softBody.size(); j++)
		{
			if (i != j)
			{
				sf::Vector2f d = { (softBody[j].x - softBody[i].x) / 100.f, (softBody[j].y - softBody[i].y) / 100.f};
				// We then get the distance from the 1st point and the 2nd.

				springLength[i][j] = Magnitude(d);
				// Then we get the spring length.
			}
		}
	}
}

void SoftBody::acceleratePoints()
{
	std::vector<sf::Vector2f> softBodyCopy = softBody;
	std::vector<sf::Vector2f> velocityCopy = velocity;

	for (int i = 0; i < softBody.size(); i++)
	{
		force[i] = { 0, 0 };

		for (int j = 0; j < softBody.size(); j++)
		{
			sf::Vector2f d = { (softBody[j].x - softBody[i].x) / 100.f, (softBody[j].y - softBody[i].y) / 100.f };

			if (Magnitude(d) != 0)
			{
				float t = atan2(d.y, d.x); // Get d.y / d.x
				float magnitude = Magnitude(d); // Get the measurements over our softbody.
				float displacement = magnitude - springLength[i][j]; // Displacement is used to change the softbody's position.

				sf::Vector2f anew = { (SpringConstant * displacement * cos(t)) / 10000.f, (SpringConstant * displacement * sin(t)) / 10000.f };
				// This makes it so that the softbody stays in tact once we run the simulation, instead of having it collapse.
			    force[i].x += anew.x;
				force[i].y += anew.y;


				// Makes it so that the force dampens once it hits something. Meaning we stop making it jiggle alot. LOL
				force[i].x -= Damping * Magnitude(d) * (velocity[i].x - velocity[j].x) / 100.f;
				force[i].y -= Damping * Magnitude(d) * (velocity[i].y - velocity[j].y) / 100.f;
			 }
			
		}
		velocityCopy[i].x += force[i].x;
		velocityCopy[i].y += force[i].y + 0.098; // GRAVITY

		softBodyCopy[i].x += velocityCopy[i].x;
		softBodyCopy[i].y += velocityCopy[i].y;
	}
	softBody = softBodyCopy;
    velocity = velocityCopy;
}

void SoftBody::checkCollisionWithWindow()
{
	for (int i = 0; i < softBody.size(); i++)
	{
		if (softBody[i].x > SCREEN_WIDTH || softBody[i].x <= 0)
		{
			velocity[i].x *= -Restitution;
			softBody[i].x = softBody[i].x > SCREEN_WIDTH / 2 ? SCREEN_WIDTH - 1 : 1;
		}

		if (softBody[i].y > SCREEN_HEIGHT || softBody[i].y <= 0)
		{
			velocity[i].y *= -Restitution;
			softBody[i].y = softBody[i].y > SCREEN_HEIGHT / 2 ? SCREEN_HEIGHT - 1 : 1;
		}
	}
}

// For collision with objects, we are using basic AABB collisions.
bool SoftBody::checkCollisionWithObjects(Objects& object, float push)
{
	for (int i = 0; i < softBody.size(); i++)
	{
	    float objectPosX = object.GetPosition().x;
	    float objectPosY = object.GetPosition().y;
	    float ObjectSizeX = object.GetSize().x;
	    float ObjectSizeY = object.GetSize().y;

		float softBodyPosX = softBody[i].x;
		float softBodyPosY = softBody[i].y;
		float softBodySizeX = GetSizeX();
		float softBodySizeY = GetSizeY();

		float deltaX = objectPosX - softBodyPosX;
		float deltaY = objectPosY - softBodyPosY;
		float intersectX = abs(deltaX) - (ObjectSizeX + softBodySizeX);
		float intersectY = abs(deltaY) - (ObjectSizeY + softBodySizeY);

		if (intersectX < 0.f && intersectY < 0.f)
		{
			push = std::clamp(push, 0.f, 1.f);

			if (intersectX > intersectY)
			{
				if (deltaX > 0.f)
				{
					velocity[i].x *= -Restitution;

					MoveForCollision(intersectX * (1.f - push), 0.f);
					object.Move(-intersectX - 1.f * push, 0.f);
				}

				else
				{
					velocity[i].x *= -Restitution;

					MoveForCollision(-intersectX * (1.f - push), 0.f);
					object.Move(intersectX + 1.f * push, 0.f);
				}
			}

			else
			{
				if (deltaY > 0.f)
				{
					velocity[i].y *= -Restitution;

					MoveForCollision(0.f, intersectY * (1.f - push));
					object.Move(0.f, -intersectY - 1.f * push);
				}

				else
				{
					velocity[i].y *= -Restitution;

					MoveForCollision(0.f, -intersectY * (1.f - push));
					object.Move(0.f, intersectY + 1.f * push);
				}
				return true;
			}
			return false;
		}
	}
}

void SoftBody::Move(float speed)
{
	for (int i = 0; i < softBody.size(); i++)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			velocity[i].y -= speed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			velocity[i].y += speed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			velocity[i].x += speed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			velocity[i].x -= speed;
	}
}

bool SoftBody::mouseHover(int PosX, int PosY)
{
	float button1PosX = button1.getPosition().x;
	float button1PosY = button1.getPosition().y;

	float button1PosXPlusbutton1Width = button1PosX + button1.getLocalBounds().width;
	float button1PosYPlusbutton1Height = button1PosY + button1.getLocalBounds().height;

	if (PosX < button1PosXPlusbutton1Width && PosY < button1PosYPlusbutton1Height && PosX > button1PosX && PosY > button1PosY)
	{
		return true;
	}

	return false;
}

bool SoftBody::mouseHover2(int PosX, int PosY)
{
	float button2PosX = button2.getPosition().x;
	float button2PosY = button2.getPosition().y;

	float button2PosXPlusbutton2Width = button2PosX + button2.getLocalBounds().width;
	float button2PosYPlusbutton2Height = button2PosY + button2.getLocalBounds().height;

	if (PosX < button2PosXPlusbutton2Width && PosY < button2PosYPlusbutton2Height && PosX > button2PosX && PosY > button2PosY)
	{
		return true;
	}

	return false;
}

bool SoftBody::mouseHover3(int PosX, int PosY)
{
	float button3PosX = button3.getPosition().x;
	float button3PosY = button3.getPosition().y;

	float button3PosXPlusbutton3Width = button3PosX + button3.getLocalBounds().width;
	float button3PosYPlusbutton3Height = button3PosY + button3.getLocalBounds().height;

	if (PosX < button3PosXPlusbutton3Width && PosY < button3PosYPlusbutton3Height && PosX > button3PosX && PosY > button3PosY)
	{
		return true;
	}

	return false;
}

bool SoftBody::mouseHover4(int PosX, int PosY)
{
	float button4PosX = button4.getPosition().x;
	float button4PosY = button4.getPosition().y;

	float button4PosXPlusbutton4Width = button4PosX + button4.getLocalBounds().width;
	float button4PosYPlusbutton4Height = button4PosY + button4.getLocalBounds().height;

	if (PosX < button4PosXPlusbutton4Width && PosY < button4PosYPlusbutton4Height && PosX > button4PosX && PosY > button4PosY)
	{
		return true;
	}

	return false;
}

void SoftBody::setButtonAndTextPos(sf::Vector2f pos)
{
	arrowRight1.setFillColor(sf::Color::Black);
	arrowRight1.setCharacterSize(30);
	arrowRight1.setString("+");

	button1.setPosition(pos);

	float xPos = (pos.x + button1.getLocalBounds().width / 2.f) - (arrowRight1.getLocalBounds().width / 2.f);
	float yPos = (pos.y + button1.getLocalBounds().height / 4.f) - (arrowRight1.getLocalBounds().height / 2.f);

	arrowRight1.setPosition({ xPos, yPos });
}

void SoftBody::setButtonAndTextPos2(sf::Vector2f pos)
{
	arrowLeft1.setFillColor(sf::Color::Black);
	arrowLeft1.setCharacterSize(30);
	arrowLeft1.setString("-");

	button2.setPosition(pos);

	float xPos = (pos.x + button2.getLocalBounds().width / 2.f) - (arrowLeft1.getLocalBounds().width / 2.f);
	float yPos = (pos.y + button2.getLocalBounds().height / 6.f) - (arrowLeft1.getLocalBounds().height / 2.f);

	arrowLeft1.setPosition({ xPos, yPos });
}

void SoftBody::setButtonAndTextPos3(sf::Vector2f pos)
{
	arrowRight2.setFillColor(sf::Color::Black);
	arrowRight2.setCharacterSize(30);
	arrowRight2.setString("+");

	button3.setPosition(pos);

	float xPos = (pos.x + button3.getLocalBounds().width / 2.f) - (arrowRight2.getLocalBounds().width / 2.f);
	float yPos = (pos.y + button3.getLocalBounds().height / 4.f) - (arrowRight2.getLocalBounds().height / 2.f);

	arrowRight2.setPosition({ xPos, yPos });
}

void SoftBody::setButtonAndTextPos4(sf::Vector2f pos)
{
	arrowLeft2.setFillColor(sf::Color::Black);
	arrowLeft2.setCharacterSize(30);
	arrowLeft2.setString("-");

	button4.setPosition(pos);

	float xPos = (pos.x + button4.getLocalBounds().width / 2.f) - (arrowLeft2.getLocalBounds().width / 2.f);
	float yPos = (pos.y + button4.getLocalBounds().height / 6.f) - (arrowLeft2.getLocalBounds().height / 2.f);

	arrowLeft2.setPosition({ xPos, yPos });
}

void SoftBody::drawLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b, sf::Color color, sf::Color color2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(a.x, a.y), color),
		sf::Vertex(sf::Vector2f(b.x, b.y), color2)
	};

	window.draw(line, 2, sf::Lines);
}

void SoftBody::Draw(sf::RenderWindow& window, sf::Color color, sf::Color color2)
{
	window.draw(button1);
	window.draw(button2);
	window.draw(button3);
	window.draw(button4);

	for (int i = 0; i < softBody.size(); i++)
	{
		drawLine(window, softBody[i], softBody[(i + 1) % softBody.size()], color, color2);
	}
}

void SoftBody::DrawText(sf::RenderWindow& window)
{
	window.draw(ResText);
	window.draw(ResTitle);
	window.draw(SpringConstText);
	window.draw(SpringConstTitle);

	window.draw(arrowRight1);
	window.draw(arrowLeft1);
	window.draw(arrowRight2);
	window.draw(arrowLeft2);
}

void SoftBody::clear()
{
	softBody.clear();
	velocity.clear();
	force.clear();
}
