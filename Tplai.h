#ifndef TPLAI_H
#define TPLAI_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include <stack>
#include <filesystem>
#include <string>
#include <locale>
#include <windows.h>
#include <thread>
#include <atomic>

const int8_t volumeMax = 5;
const std::set<std::string> validExtensions = { ".ogg", ".mp3", ".wav", ".flac", ".aac", ".m4a" };

namespace fs = std::filesystem;
const std::string folderPath = "Tmusic";

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

	void play();

	std::stack<std::string> _lastFiles;
	sf::RectangleShape _volume[volumeMax];
	int8_t _volumeSetting;
	float _currVolume;
	sf::RectangleShape _V_Up; // Volume Up
	sf::RectangleShape _V_Dw; // Volume Down

	std::string _filePath;

	sf::Font _font;
	sf::Text _display_CurrM;
	sf::Text _display_Dur;
	std::string DisplayFix(std::string& _disFile);

	sf::RectangleShape _P_Button; // Pause
	sf::RectangleShape _N_Button; // Next
	sf::RectangleShape _B_Button; // Back

	sf::RectangleShape _panel;

	sf::RectangleShape _addM;

	void OpenFileExplorer();

	bool _startM;
	bool _repeat;
	bool _back;
	bool _isLeftMousePressed;

	sf::Texture P_Texture, Pl_Texture, N_Texture, B_Texture;
	sf::Texture V_Up_Texture, V_Dw_Texture, Add_Texture, V_Mute_Texture;

	void initWindow();
	void initTplai();
	void initPanel();
	void initB();
	void initV();
	void initFileOp();
};

#endif // !TPLAI_H