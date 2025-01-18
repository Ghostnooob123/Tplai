#ifndef FRONTEND_H
#define FRONTEND_H

#include <SFML/Graphics.hpp>
#include "backend.h"

class Frontend
{
public:
	Frontend();
	~Frontend();

	bool running();

	void update();
	void render();

private:

	sf::RenderWindow* _window;
	sf::Event _event;

	sf::Vector2f _mousePos;

	float _x;
	float _y;

	void updateMousePos();
	void pollEvents();


	void initWindow();
};

#endif // !FRONTEND_H

