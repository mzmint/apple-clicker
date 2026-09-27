#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>

void saveGame(double apples, double aps, double apc) {

    nlohmann::json save;

    save["apples"] = apples;
    save["aps"] = aps;
    save["apc"] = apc;

    std::ofstream file("save.json");

    file << save.dump(4);
}

void loadGame(double& apples, double& aps, double& apc)
{
    std::ifstream file("save.json");

    if (!file)
        return;

    nlohmann::json save;

    file >> save;

    apples = save.value("apples", 0.0);
    aps = save.value("aps", 1.0);
    apc = save.value("apc", 1.0);
}

int main() {

    sf::Clock clock;
    sf::Clock apsClock;

    double apples = 0;
    double aps = 0;
    double apc = 1;

    loadGame(apples, aps, apc);

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

    sf::Text apsText(font);
    apsText.setString("Apples per second: 0");
    apsText.setCharacterSize(50);
    apsText.setFillColor(sf::Color::Black);
    apsText.setPosition({50.f, 120.f});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                saveGame(apples, aps, apc);
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
            float elapsed = apsClock.restart().asSeconds();
            apples += aps * elapsed;

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
        window.draw(apsText);
        window.draw(menu);

        window.display();
    }

    return 0;
}