#ifndef TPLAI_H
#define TPLAI_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <stack>
#include <filesystem>
#include <memory>
#include <set>
#include <locale>

const std::set<std::string> validExtensions = { ".ogg", ".mp3", ".wav", ".flac", ".aac", ".m4a" };

namespace fs = std::filesystem;
const std::string folderPath = "music";

class Tplai
{
public:
	Tplai();
	~Tplai();


	bool running();

	void update();
	void render();

private:
	sf::RenderWindow* _window;

	sf::Music _currM;
	sf::Event _event;

	sf::Vector2f _mousePos;

	float _x;
	float _y;

	void updateMousePos();
	void pollEvents();

	void reload();
	void play();

	std::shared_ptr< std::set<std::string>> _knownFiles;
	std::stack<std::string> _lastFiles;
	//std::set<std::string>::iterator it;

	sf::Font font;

	bool _startM;
	bool _repeat;

	void initWindow();
	void initTplai();
};

#endif // !TPLAI_H