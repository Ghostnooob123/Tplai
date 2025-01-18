#include "Frontend.h"

Frontend::Frontend()
{
	this->initWindow;
}

Frontend::~Frontend()
{
	delete this->_window;
	this->_window = nullptr;
}

bool Frontend::running()
{
	if (this->_window->isOpen())
	{
		return true;
	}
	return false;
}

void Frontend::updateMousePos()
{
	sf::Vector2i mouseWindow = sf::Mouse::getPosition(*this->_window);
	this->_mousePos = this->_window->mapPixelToCoords(mouseWindow);
}

void Frontend::pollEvents()
{
	while (this->_window->pollEvent(_event)) {
		if (sf::Keyboard::Escape == _event.key.code)
		{
			this->_window->close();
		}
	}
}

void Frontend::update()
{

}

void Frontend::render()
{
	this->_window->clear(sf::Color::Magenta);

	this->_window->display();
}

void Frontend::initWindow()
{
	this->_window = new sf::RenderWindow(sf::VideoMode(1200, 700), "Tplay");
	this->_window->setFramerateLimit(60);
}
