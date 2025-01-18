#ifndef BACKEND_H
#define BACKEND_H

#include <SFML/Audio.hpp>
#include <filesystem>
#include <set>

namespace fs = std::filesystem;
const std::string folderPath = "music";

class Backend
{
public:
	Backend();
	~Backend();

	void reload();

	void play();

private:
	std::set<std::string> knownFiles;

};

#endif // !BACKEND_H
