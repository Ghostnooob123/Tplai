#include "Tplai.h"

Tplai::Tplai()
{
    this->initWindow();
    this->initTplai();
}

Tplai::~Tplai()
{
    delete this->_window;
    this->_window = nullptr;
}

bool Tplai::running()
{
    if (this->_window->isOpen())
    {
        return true;
    }
    return false;
}

void Tplai::update()
{
    this->updateMousePos();
    this->pollEvents();
    this->reload();

    if (!this->_startM)
    {
        this->_startM = true;
        this->play();
    }
}

void Tplai::render()
{
    this->_window->clear(sf::Color::Black);

    this->_window->display();
}

void Tplai::updateMousePos()
{
    sf::Vector2i mouseWindow = sf::Mouse::getPosition(*this->_window);
    this->_mousePos = this->_window->mapPixelToCoords(mouseWindow);
}

void Tplai::pollEvents()
{
    while (this->_window->pollEvent(this->_event)) {
        if (sf::Keyboard::Escape == this->_event.key.code)
        {
            this->_window->close();
        }
        if (this->_event.type == sf::Event::KeyPressed && 
            this->_event.key.code == sf::Keyboard::Right) {
            this->_currM.stop();
            break;
        }
        /*if (this->_event.type == sf::Event::KeyPressed &&
            this->_event.key.code == sf::Keyboard::Left) {
          
            this->_currM.stop();
          
            break;
        }*/
        if (this->_event.type == sf::Event::KeyPressed && 
            this->_event.key.code == sf::Keyboard::Space) {
            if (this->_currM.getStatus() == sf::Music::Playing)
            {
                this->_currM.pause();
            }
            else
            {
                this->_currM.play();
            }
            break;
        }
    }
}

void Tplai::reload()
{
    try
    {
        std::setlocale(LC_ALL, "en_US.UTF-8");

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (!entry.is_regular_file()) {
                continue;
            }
            std::string ext = entry.path().extension().string();
            if (validExtensions.find(ext) == validExtensions.end()) {
                continue;
            }

            auto find = _knownFiles->find(entry.path().string());
            if (find != _knownFiles->end())
            {

            }

            /*if (entry.is_regular_file() && entry.path().extension().string() == ".ogg") {
                
            }*/
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Tplai::play()
{
    std::setlocale(LC_ALL, "en_US.UTF-8");

    while (this->_repeat)               
    {
        for (auto it = this->_knownFiles->begin(); it 
            != this->_knownFiles->end(); it++) {

            std::string filePath = it->c_str();
            std::string displayFile = filePath.substr(6, filePath.length() - 10);
            std::cout << filePath << '\n';
            std::cout << displayFile << '\n';

            this->_currM.openFromFile(filePath);
            this->_currM.setVolume(1000.0f);
            this->_currM.play();

            while (this->_currM.getStatus() == sf::Music::Playing ||
                this->_currM.getStatus() == sf::Music::Paused)
            {
                this->update();
                this->render();
            }

            this->_lastFiles.push(filePath);
            /*if (entry.is_regular_file() && entry.path().extension() == ".ogg") {
               
            }*/
        }
    }
}

void Tplai::initWindow()
{
    this->_window = new sf::RenderWindow(sf::VideoMode(1200, 700), "Tplai");
    this->_window->setFramerateLimit(60);
}

void Tplai::initTplai()
{
    this->_knownFiles = std::make_shared<std::set<std::string>>();

    this->_startM = false;
    this->_repeat = true;

    sf::Font font;
    if (!font.loadFromFile("path/to/your/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }
}
