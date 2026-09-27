#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>


int main() {

    sf::Clock clock;

    int apples = 0;

    std::ifstream saveFile("save.txt");

    if (saveFile) {
        saveFile >> apples;
    }

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


    //text

    sf::Text appleText(font);

    appleText.setString("Apples: 0");
    appleText.setCharacterSize(50);
    appleText.setFillColor(sf::Color::Black);
    appleText.setPosition({50.f, 50.f});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                std::ofstream saveFile("save.txt");
                saveFile << apples;
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
                        apples++;
                        appleText.setString("Apples: " + std::to_string(apples));
                    }
                }
            }
            float time = clock.getElapsedTime().asSeconds();

            float rotation = std::sin(time * 2.f) * 10.f;

            apple.setRotation(sf::degrees(rotation));
        }
        float time = clock.getElapsedTime().asSeconds();
        apple.setRotation(
            sf::degrees(std::sin(time * 2.f) * 8.f)
        );

        window.clear(sf::Color(200, 200, 200));

        window.draw(apple);
        window.draw(appleText);
        window.draw(menu);

        window.display();
    }

    return 0;
}