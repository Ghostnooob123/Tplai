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
    if (!this->running())
    {
        this->_currM.stop();
        return;
    }

    this->updateMousePos();
    this->pollEvents();

    if (!this->_startM)
    {
        this->_startM = true;
        this->play();
    }
}

void Tplai::render()
{
    this->_window->clear(sf::Color(166, 165, 162));

    this->_window->draw(this->_P_Button);
    this->_window->draw(this->_N_Button);
    this->_window->draw(this->_B_Button);
    this->_window->draw(this->_display_Dur);
    this->_window->draw(this->_V_Up);
    this->_window->draw(this->_V_Dw);

    this->_window->draw(this->_addM);

    this->_window->draw(this->_panel);
    this->_window->draw(this->_display_CurrM);
    this->_window->draw(this->_display_Dur);

    for (size_t i = 0; i < volumeMax; i++)
    {
        this->_window->draw(this->_volume[i]);
    }

    this->_window->display();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));

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
            break;
        }
        if (this->_event.type == sf::Event::Closed) {
            this->_window->close();
            break;
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isLeftMousePressed &&
            this->_N_Button.getGlobalBounds().contains(this->_mousePos)) {
            this->_currM.stop();
            isLeftMousePressed = true;
            break;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isLeftMousePressed &&
            this->_B_Button.getGlobalBounds().contains(this->_mousePos)) {
          
            this->_currM.stop();
            this->_back = true;
            if (this->_lastFiles.size() != 1)
            {
                this->_lastFiles.pop();
            }
            isLeftMousePressed = true;
            break;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isLeftMousePressed &&
            this->_P_Button.getGlobalBounds().contains(this->_mousePos)) {
            if (this->_currM.getStatus() == sf::Music::Playing)
            {
                this->_currM.pause();
            }
            else
            {
                this->_currM.play();
            }
            isLeftMousePressed = true;
            break;
        }

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
            isLeftMousePressed = true;
            break;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isLeftMousePressed &&
            this->_V_Up.getGlobalBounds().contains(this->_mousePos)) {
            if (this->_currVolume < 100.0f && this->_volumeSetting < 5)
            {
                isLeftMousePressed = true;
                this->_currVolume += 10.0f;
                this->_volume[this->_volumeSetting++].setFillColor(sf::Color(138, 137, 134));
            }
            break;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isLeftMousePressed &&
            this->_V_Dw.getGlobalBounds().contains(this->_mousePos)) {
            if (this->_currVolume > 0.0f && this->_volumeSetting >= 0)
            {
                isLeftMousePressed = true;
                this->_currVolume -= 10.0f;
                this->_volume[--this->_volumeSetting].setFillColor(sf::Color::Transparent);
            }
            break;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isLeftMousePressed &&
            this->_addM.getGlobalBounds().contains(this->_mousePos)) {
            this->OpenFileExplorer();
            isLeftMousePressed = true;
            break;
        }

        // Release the button ( Prevent from multiple clicks at one time )
        if (this->_event.type == sf::Event::MouseButtonReleased) {
            if (this->_event.mouseButton.button == sf::Mouse::Left) {
                isLeftMousePressed = false;
            }
        }
    }
}

void Tplai::play()
{
    std::setlocale(LC_ALL, "en_US.UTF-8");

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        std::string ext = entry.path().extension().string();
        if (!entry.is_regular_file() || validExtensions.find(ext) == validExtensions.end()) {
            continue;
        }

        if (this->_back)
        {
            if (this->_lastFiles.size() == 1)
            {
                if (!this->_lastFiles.empty() && entry.path().string() != this->_lastFiles.top())
                {
                    continue;
                }
                this->_filePath = entry.path().string();
                this->_back = false;
            }
            else
            {
                if (!this->_lastFiles.empty() && entry.path().string() != this->_lastFiles.top())
                {
                    continue;
                }
                this->_filePath = entry.path().string();
                this->_lastFiles.pop();
                this->_back = false;
            }
        }
        else
        {
            this->_filePath = entry.path().string();
        }

        std::string _displayFile = this->_filePath.substr(6, this->_filePath.length() - 10);
        _displayFile = this->DisplayFix(_displayFile);

        this->_display_CurrM.setString(_displayFile);

        this->_currM.openFromFile(this->_filePath);
        this->_currM.play();

        this->_lastFiles.push(this->_filePath);

        sf::Time totalDuration = this->_currM.getDuration();
        int totalSeconds = static_cast<int>(totalDuration.asSeconds());
        int totalMinutes = totalSeconds / 60;
        totalSeconds %= 60;

        while (this->_currM.getStatus() == sf::Music::Playing ||
            this->_currM.getStatus() == sf::Music::Paused)
        {
            this->_currM.setVolume(this->_currVolume);

            sf::Time currentTime = this->_currM.getPlayingOffset();
            int currSeconds = static_cast<int>(currentTime.asSeconds());
            int currMinutes = currSeconds / 60;
            currSeconds %= 60;

            std::string curDur = std::to_string(currMinutes) + ":" +
                (currSeconds < 10 ? "0" : "") + std::to_string(currSeconds) +
                " - " + std::to_string(totalMinutes) + ":" +
                (totalSeconds < 10 ? "0" : "") + std::to_string(totalSeconds);

            this->_display_Dur.setString(curDur);

            this->update();
            this->render();
        }
        if (!this->running())
        {
            return;
        }
    }
    _startM = false;
}

std::string Tplai::DisplayFix(std::string& _disFile)
{
    for (auto& c : _disFile)
    {
        if (!std::isprint(c))
        {
            c = '.';
        }
    }
    if (_disFile.length() > 18)
    {
        _disFile = _disFile.substr(0, 18);
        _disFile += "...";
        this->_display_CurrM.setString(_disFile.substr(1, 3));
    }

    return _disFile;
}

void Tplai::OpenFileExplorer()
{
    ShellExecute(NULL, "open", "explorer.exe", "music", NULL, SW_SHOWNORMAL);
}

void Tplai::initWindow()
{
    this->_window = new sf::RenderWindow(sf::VideoMode(550, 400), "Tplai", sf::Style::Close);
    this->_window->setFramerateLimit(60);

    this->_x = this->_window->getSize().x / 2.0f;
    this->_y = this->_window->getSize().y / 2.0f;
}

void Tplai::initTplai()
{
    this->_startM = false;
    this->_repeat = true;

    this->initV();

    this->initPanel();

    this->initB();

    this->initFileOp();
}

void Tplai::initPanel()
{
    this->_panel.setFillColor(sf::Color::Transparent);
    this->_panel.setOutlineColor(sf::Color::Black);
    this->_panel.setOutlineThickness(3.0f);
    this->_panel.setSize(sf::Vector2f(300.0f, 100.0f));
    this->_panel.setPosition(sf::Vector2f(this->_x - 150.0f, this->_y - 20.0f));

    if (!this->_font.loadFromFile("font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    this->_display_CurrM.setCharacterSize(25);
    this->_display_CurrM.setFillColor(sf::Color::Black);
    this->_display_CurrM.setFont(this->_font);
    this->_display_CurrM.setOutlineThickness(0.5f);
    this->_display_CurrM.setOutlineColor(sf::Color::Black);
    this->_display_CurrM.setPosition(sf::Vector2f(this->_panel.getPosition().x + 5.0f,
        this->_panel.getPosition().y + 2.0f));

    this->_display_Dur.setCharacterSize(25);
    this->_display_Dur.setFillColor(sf::Color::Black);
    this->_display_Dur.setFont(this->_font);
    this->_display_Dur.setOutlineThickness(0.5f);
    this->_display_Dur.setOutlineColor(sf::Color::Black);
    this->_display_Dur.setPosition(sf::Vector2f(this->_panel.getPosition().x + 5.0f,
        this->_panel.getPosition().y + 65.0f));
}

void Tplai::initB()
{
    this->_P_Button.setFillColor(sf::Color::White);
    this->_P_Button.setOutlineColor(sf::Color::Black);
    this->_P_Button.setOutlineThickness(3.0f);
    this->_P_Button.setSize(sf::Vector2f(90.0f, 50.0f));
    this->_P_Button.setPosition(sf::Vector2f(this->_x - 45.0f, this->_y + 100.0f));

    this->_N_Button.setFillColor(sf::Color::White);
    this->_N_Button.setOutlineColor(sf::Color::Black);
    this->_N_Button.setOutlineThickness(3.0f);
    this->_N_Button.setSize(sf::Vector2f(50.0f, 50.0f));
    this->_N_Button.setPosition(sf::Vector2f(this->_x + 55.0f, this->_y + 100.0f));

    this->_B_Button.setFillColor(sf::Color::White);
    this->_B_Button.setOutlineColor(sf::Color::Black);
    this->_B_Button.setOutlineThickness(3.0f);
    this->_B_Button.setSize(sf::Vector2f(50.0f, 50.0f));
    this->_B_Button.setPosition(sf::Vector2f(this->_x - 105.0f, this->_y + 100.0f));
}

void Tplai::initV()
{
    this->_volumeSetting = 0;
    this->_currVolume = 10.0f;

    float gap = 0.0f;
    float sizeGap = 0.0f;
    for (size_t i = 0; i < volumeMax; i++)
    {
        sf::RectangleShape tempShape;
        tempShape.setFillColor(sf::Color::Transparent);
        tempShape.setOutlineColor(sf::Color::Black);
        tempShape.setOutlineThickness(3.0f);
        tempShape.setSize(sf::Vector2f(15.0f, 15.0f + sizeGap));
        tempShape.setPosition(sf::Vector2f((this->_x + 20.0f) + gap * 2.3f, (this->_y + 55.0f) - gap));
        gap += 10.0f;
        sizeGap += 10.0f;
        this->_volume[i] = tempShape;
    }
    this->_volume[this->_volumeSetting++].setFillColor(sf::Color(138, 137, 134));

    this->_V_Up.setFillColor(sf::Color::White);
    this->_V_Up.setOutlineColor(sf::Color::Black);
    this->_V_Up.setOutlineThickness(3.0f);
    this->_V_Up.setSize(sf::Vector2f(30.0f, 30.0f));
    this->_V_Up.setPosition(sf::Vector2f(this->_x + 220.0f, this->_y + 100.0f));

    this->_V_Dw.setFillColor(sf::Color::White);
    this->_V_Dw.setOutlineColor(sf::Color::Black);
    this->_V_Dw.setOutlineThickness(3.0f);
    this->_V_Dw.setSize(sf::Vector2f(30.0f, 30.0f));
    this->_V_Dw.setPosition(sf::Vector2f(this->_x + 160.0f, this->_y + 100.0f));
}

void Tplai::initFileOp()
{
    this->_addM.setFillColor(sf::Color::White);
    this->_addM.setOutlineColor(sf::Color::Black);
    this->_addM.setOutlineThickness(2.3f);
    this->_addM.setSize(sf::Vector2f(60.0f, 40.0f));
    this->_addM.setPosition(sf::Vector2f(this->_x - 250.0f, this->_y - 170.0f));
}
