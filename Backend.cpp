#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"

Backend::Backend()
{

}

Backend::~Backend()
{
	delete this->m_window;
}

void Backend::reload()
{
    try
    {
        for (const auto& entry : fs::directory_iterator(folderPath)) {

            if (entry.is_regular_file() && entry.path().extension().string() == ".ogg") {
                knownFiles.insert(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Backend::play()
{
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".ogg") {
            std::string filePath = entry.path().string();

            sf::Music currM;
            if (!currM.openFromFile(filePath))
                return -1; // error
            currM.setVolume(1000.0f);
            currM.play();


            // Main loop
            while (currM.getStatus() == sf::Music::Playing) {

                sf::Event event;
                while (window.isOpen()) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();  // Close the window when it's closed by the user
                        }

                        // Check if the spacebar is pressed
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                            currM.stop();  // Stop the music
                            std::cout << "Music skipped\n";
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}
