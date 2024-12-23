#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Objects.hpp"

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 900

// Inline makes the program faster.

inline double Magnitude(sf::Vector2f a) // This is used to measure the size or the amount of the softbody.
{
	double Mag = sqrt(a.x * a.x + a.y * a.y);
	return Mag;
}

inline sf::Vector2f Unit(sf::Vector2f a) // This is used to used to measure the physical quantity, such as mass or length
{
	sf::Vector2f b;
	double Mag = Magnitude(a);

	if (Mag == 0)
		return { 0, 0 };

	b.x = a.x / Mag;
	b.y = a.y / Mag;

	return b;
}

inline double MakeDot(sf::Vector2f a, sf::Vector2f b)
{
	double Dot = a.x * b.x + a.y * b.y;
	return Dot;
}

class SoftBody
{
public:
	SoftBody(float Restitution, float SpringConstant, float Damping);

	void AddPoint(float x, float y);
	void acceleratePoints();
	void checkCollisionWithWindow();
	bool checkCollisionWithObjects(Objects& object, float push);
	void Move(float speed);

	bool mouseHover(int PosX, int PosY);
	bool mouseHover2(int PosX, int PosY);
	bool mouseHover3(int PosX, int PosY);
	bool mouseHover4(int PosX, int PosY);

	void MoveForCollision(float dx, float dy)
	{
		for (int i = 0; i < softBody.size(); i++)
		{
			softBody[i].x += dx;
			softBody[i].y += dy;
		}
	}

	float GetSizeX()
	{
		for (int i = 0; i < softBody.size(); i++)
		{
			for (int j = 0; j < softBody.size(); j++)
			{
				return softBody[i].x - softBody[j].x;
			}
		}
	}

	float GetSizeY()
	{
		for (int i = 0; i < softBody.size(); i++)
		{
			for (int j = 0; j < softBody.size(); j++)
			{
				return softBody[i].y - softBody[j].y;
			}
		}
	}

	void setFont(sf::Font& font)
	{
		ResText.setFont(font);
		ResTitle.setFont(font);
		SpringConstText.setFont(font);
		SpringConstTitle.setFont(font);

		arrowRight1.setFont(font);
		arrowLeft1.setFont(font);
		arrowRight2.setFont(font);
		arrowLeft2.setFont(font);
	}

	void setBGColor(sf::Color color)
	{
		button1.setFillColor(color);
	}

	void setBGColor2(sf::Color color)
	{
		button2.setFillColor(color);
	}

	void setBGColor3(sf::Color color)
	{
		button3.setFillColor(color);
	}

	void setBGColor4(sf::Color color)
	{
		button4.setFillColor(color);
	}

	void setButtonAndTextPos(sf::Vector2f pos);
	void setButtonAndTextPos2(sf::Vector2f pos);
	void setButtonAndTextPos3(sf::Vector2f pos);
	void setButtonAndTextPos4(sf::Vector2f pos);

	sf::Vector2f returnDrag()
	{
		return (softBody[0]);
	}

	void drawLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b, sf::Color color, sf::Color color2);
	void Draw(sf::RenderWindow& window, sf::Color color, sf::Color color2);
	void DrawText(sf::RenderWindow& window);

	void clear();

	void IncrementRes()
	{
		Restitution += 0.1;
		ResText.setString(std::to_string(Restitution));
	}

	void DecrementRes()
	{
		Restitution -= 0.1;
		ResText.setString(std::to_string(Restitution));
	}

	void IncrementSpringConst()
	{
		SpringConstant += 1000;
		SpringConstText.setString(std::to_string(SpringConstant));
	}

	void DecrementSpringConst()
	{
		SpringConstant -= 1000;
		SpringConstText.setString(std::to_string(SpringConstant));
	}

private:
	std::vector<sf::Vector2f> softBody;
	std::vector<sf::Vector2f> velocity;
	std::vector<sf::Vector2f> force;

	float springLength[200][200];

	float Restitution;
	float SpringConstant;
	float Damping;


	sf::RectangleShape button1;
	sf::RectangleShape button2;

	sf::Text ResText;
	sf::Text ResTitle;

	sf::Text arrowRight1;
	sf::Text arrowLeft1;

	sf::RectangleShape button3;
	sf::RectangleShape button4;

	sf::Text SpringConstText;
	sf::Text SpringConstTitle;

	sf::Text arrowRight2;
	sf::Text arrowLeft2;
};
