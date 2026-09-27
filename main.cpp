#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>

void saveGame(double apples, double aps, double apc, double ajc) {

    nlohmann::json save;

    save["apples"] = apples;
    save["aps"] = aps;
    save["apc"] = apc;
    save["ajc"] = ajc;

    std::ofstream file("save.json");

    file << save.dump(4);
}

void loadGame(double& apples, double& aps, double& apc, double& ajc)
{
    std::ifstream file("save.json");

    if (!file)
        return;

    nlohmann::json save;

    file >> save;

    apples = save.value("apples", 0.0);
    aps = save.value("aps", 0.0);
    apc = save.value("apc", 1.0);
    ajc = save.value("ajc", 10.0);
}

int main() {

    sf::Clock clock;
    sf::Clock apsClock;

    double apples = 0;
    double aps = 0;
    double apc = 1;
    double ajc = 10;

    loadGame(apples, aps, apc, ajc);

    sf::RenderWindow window(
        sf::VideoMode({1920, 1080}),
        "Apple Clicker"
    );

    //textures

    sf::Font font;

    if (!font.openFromFile("assets/apple_days.ttf")) {
        return 1;
    }

    sf::Texture appleTexture;

    if (!appleTexture.loadFromFile("assets/apple.png")) {
        return 1;
    }

    sf::Texture menuTexture;

    if (!menuTexture.loadFromFile("assets/menu.png")) {
        return 1;
    }

    //appel

    sf::Sprite apple(appleTexture);

    sf::Vector2u textureSize = appleTexture.getSize();

    float scale = std::min(
        200.f / textureSize.x,
        200.f / textureSize.y
    );

    apple.setScale({scale, scale});
    apple.setPosition({960.f, 640.f});
    apple.setOrigin({
        apple.getLocalBounds().size.x / 2.f,
        apple.getLocalBounds().size.y / 2.f
    });

    //menu

    sf::Sprite menu(menuTexture);

    sf::Vector2u textureSizeMenu = menuTexture.getSize();

    float menuscale = std::min(
        500.f / textureSizeMenu.x,
        1040.f / textureSizeMenu.y
    );

    menu.setScale({menuscale, menuscale});
    menu.setPosition({1630.f, 540.f});
    menu.setOrigin({
        menu.getLocalBounds().size.x / 2.f,
        menu.getLocalBounds().size.y / 2.f
    });

    //menu buttons

    sf::RectangleShape clickbtn1({360.f, 80.f});
    clickbtn1.setPosition({1630.f, 150.f});
    clickbtn1.setOrigin({180.f, 40.f});

    sf::Text cb1Text(font);
    cb1Text.setString("Apple Juice: 10 ");
    cb1Text.setCharacterSize(32);
    cb1Text.setFillColor(sf::Color::Black);
    cb1Text.setPosition({1472.f, 132.f});

    //text

    sf::Text appleText(font);
    appleText.setCharacterSize(50);
    appleText.setFillColor(sf::Color::Black);
    appleText.setPosition({50.f, 50.f});

    sf::Text apsText(font);
    apsText.setCharacterSize(50);
    apsText.setFillColor(sf::Color::Black);
    apsText.setPosition({50.f, 120.f});

    sf::Text apcText(font);
    apcText.setCharacterSize(50);
    apcText.setFillColor(sf::Color::Black);
    apcText.setPosition({50.f, 190.f});

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(0) << apples;
    appleText.setString("Apples: " + stream.str());
    std::ostringstream stream2;
    stream2 << std::fixed << std::setprecision(0) << apc;
    apcText.setString("Apples per click: " + stream2.str());
    std::ostringstream stream3;
    stream3 << std::fixed << std::setprecision(0) << aps;
    apsText.setString("Apples per second: " + stream3.str());
    std::ostringstream stream4;
    stream4 << std::fixed << std::setprecision(0) << ajc;
    cb1Text.setString("Apple Juice: " + stream4.str());


    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                saveGame(apples, aps, apc, ajc);
                window.close();
            }

            if (const auto* mouse =
                    event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = {
                        static_cast<float>(mouse->position.x),
                        static_cast<float>(mouse->position.y)
                    };

                    if (apple.getGlobalBounds().contains(mousePos)) {
                        apples += apc;
                        std::ostringstream stream;
                        stream << std::fixed << std::setprecision(0) << apples;
                        appleText.setString("Apples: " + stream.str());
                    }
                }
            }

            if (const auto* mouse =
                    event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = {
                        static_cast<float>(mouse->position.x),
                        static_cast<float>(mouse->position.y)
                    };

                    if (clickbtn1.getGlobalBounds().contains(mousePos) && apples >= ajc) {
                        apc ++;
                        apples -= ajc;
                        ajc *= 2;
                        std::ostringstream stream;
                        stream << std::fixed << std::setprecision(0) << apples;
                        appleText.setString("Apples: " + stream.str());
                        std::ostringstream stream2;
                        stream2 << std::fixed << std::setprecision(0) << apc;
                        apcText.setString("Apples per click: " + stream2.str());
                        std::ostringstream stream3;
                        stream3 << std::fixed << std::setprecision(0) << ajc;
                        cb1Text.setString("Apple Juice: " + stream3.str());
                    }
                }
            }
            float time = clock.getElapsedTime().asSeconds();
            float rotation = std::sin(time * 2.f) * 10.f;
            apple.setRotation(sf::degrees(rotation));
        }
        float elapsed = apsClock.restart().asSeconds();
        apples += aps * elapsed;

        float time = clock.getElapsedTime().asSeconds();
        apple.setRotation(
            sf::degrees(std::sin(time * 2.f) * 8.f)
        );

        window.clear(sf::Color(200, 200, 200));

        window.draw(apple);
        window.draw(appleText);
        window.draw(apsText);
        window.draw(apcText);
        window.draw(menu);
        window.draw(clickbtn1);
        window.draw(cb1Text);

        window.display();
    }

    return 0;
}