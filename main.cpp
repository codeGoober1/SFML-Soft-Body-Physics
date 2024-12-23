#include "SoftBody.hpp"
#include "Objects.hpp"

#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Soft Body Physics", sf::Style::Close | sf::Style::Titlebar);
	sf::Event event;

	window.setFramerateLimit(120);

	sf::Font font;
	font.loadFromFile("Font3.ttf");

	sf::Text Instructions;
	Instructions.setFont(font);
	Instructions.setCharacterSize(15);
	Instructions.setPosition(400, 0);
	Instructions.setFillColor(sf::Color::White);
	Instructions.setString("Use the mouse to make points and press space to make the simulation begin. Press 'C' to clear / destroy the soft body.");

	sf::Music music;
	//music.openFromFile("Music Jam.mp3");
        // MUSIC IS OPTIONAL

	SoftBody body(0.8f, 10000.f, 1.f);

	body.setFont(font);
	body.setButtonAndTextPos({ 170, 30 });
	body.setButtonAndTextPos2({ 40, 30 });
	body.setButtonAndTextPos3({ 203, 100 });
	body.setButtonAndTextPos4({ 40, 100 });

	music.setPitch(1);

	music.play();
	music.setLoop(true);

	std::vector<Objects> object;

	object.push_back(Objects(300.f, 100.f, 800.f, 150.f, sf::Color(150, 150, 150)));
	object.push_back(Objects(800.f, 150.f, 800.f, 450.f, sf::Color(150, 150, 150)));

	bool move = false, enable = true;

	while (window.isOpen())
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		while (window.pollEvent(event))
		{
			if (event.type == event.Closed)
			{
				window.close();
			}

			else if (enable)
			{
				if (event.type == event.MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						body.AddPoint(mousePos.x, mousePos.y);
					}
				}

				if (event.type == event.KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Space)
					{
						enable = false;
					}
				}
			}

			else
			{
				if (event.type == event.KeyPressed)
				{
					if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up ||
						event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down || 
						event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right || 
						event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left )
					{
						move = true;
					}

				}

				if (event.type == event.KeyReleased)
				{
					move = false;
				}
			}

			if (event.type == event.KeyPressed)
			{
				if (event.key.code == sf::Keyboard::C)
				{
					body.clear();
					enable = true;
				}
			}

			if (event.type == event.MouseMoved)
			{
				// MOUSE HOVER 1
				if (body.mouseHover(mousePos.x, mousePos.y))
				{
					body.setBGColor(sf::Color(200, 200, 200));
				}

				else
				{
					body.setBGColor(sf::Color::White);
				}

				// MOUSE HOVER 2
			    if (body.mouseHover2(mousePos.x, mousePos.y))
			    {
				    body.setBGColor2(sf::Color(200, 200, 200));
			    }

			    else
			    {
				    body.setBGColor2(sf::Color::White);
			    }

				// MOUSE HOVER 3
				if (body.mouseHover3(mousePos.x, mousePos.y))
				{
					body.setBGColor3(sf::Color(200, 200, 200));
				}

				else
				{
					body.setBGColor3(sf::Color::White);
				}

				// MOUSE HOVER 4
				if (body.mouseHover4(mousePos.x, mousePos.y))
				{
					body.setBGColor4(sf::Color(200, 200, 200));
				}

				else
				{
					body.setBGColor4(sf::Color::White);
				}
			}

			if (event.type == event.MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (body.mouseHover(mousePos.x, mousePos.y))
					{
						body.IncrementRes();
					}

					else if (body.mouseHover2(mousePos.x, mousePos.y))
					{
						body.DecrementRes();
					}

					else if (body.mouseHover3(mousePos.x, mousePos.y))
					{
						body.IncrementSpringConst();
					}

					else if (body.mouseHover4(mousePos.x, mousePos.y))
					{
						body.DecrementSpringConst();
					}
				}
			}
		}

		if (!enable)
		{
			if (move)
			{
				body.Move(0.25f);
			}

			body.checkCollisionWithWindow();
			body.acceleratePoints();

			for (auto objects : object)
			{
				body.checkCollisionWithObjects(objects, 0.f);
			}
		}

		window.clear(sf::Color(60, 60, 60));
		body.Draw(window, sf::Color(255, 128, 128), sf::Color::Green);
		body.DrawText(window);
		window.draw(Instructions);

		for (auto objects : object)
		{
			objects.Draw(window);
		}

		window.display();
	}
}

