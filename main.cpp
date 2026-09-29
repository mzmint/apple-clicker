#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>

std::string formatNumber(double number)
{
    std::ostringstream out;

    if (number >= 1'000'000'000'000.0) {
        out << std::fixed << std::setprecision(1)
            << number / 1'000'000'000'000.0 << "T";
    } else if (number >= 1'000'000'000.0) {
        out << std::fixed << std::setprecision(1)
            << number / 1'000'000'000.0 << "B";
    } else if (number >= 1'000'000.0) {
        out << std::fixed << std::setprecision(1)
            << number / 1'000'000.0 << "M";
    } else {
        out << std::fixed << std::setprecision(0) << number;
    }

    return out.str();
}

double parseNumber(const std::string& text)
{
    if (text.empty())
        return 0;

    char suffix = text.back();

    double number;

    try
    {
        number = std::stod(text.substr(0, text.size() - 1));
    }
    catch (...)
    {
        return 0;
    }

    switch (suffix) {
        case 'M':
            return number * 1'000'000.0;
        case 'B':
            return number * 1'000'000'000.0;
        case 'T':
            return number * 1'000'000'000'000.0;
        default:
            return std::stod(text);
    }
}

void saveGame(double apples, double aps, double apc, double ajc, double atc, double afc, bool isdiagon, int ftf, double apiec) {

    nlohmann::json save;

    save["apples"] = apples;
    save["aps"] = aps;
    save["apc"] = apc;
    save["ajc"] = ajc;
    save["atc"] = atc;
    save["afc"] = afc;
    save["isdiagon"] = isdiagon;
    save["ftf"] = ftf;
    save["apiec"] = apiec;

    std::ofstream file("save.json");

    file << save.dump(4);
}

void loadGame(double& apples, double& aps, double& apc, double& ajc, double& atc, double& afc, bool& isdiagon, int& ftf, double& apiec)
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
    atc = save.value("atc", 20.0);
    afc = save.value("afc", 400.0);
    isdiagon = save.value("isdiagon", false);
    ftf = save.value("ftf", 2);
    apiec = save.value("apiec", 1600);
}

int main() {
    sf::Music music;

    if (!music.openFromFile("assets/Appels.mp3")) {
        return 1;
    }

    music.setLooping(true);
    music.play();

    sf::Clock clock;
    sf::Clock apsClock;

    double apples = 0;
    double aps = 0;
    double apc = 1;
    double ajc = 10;
    double atc = 20;
    double afc = 400;
    double apiec = 1600;
    bool isdiagon = false;
    int ftf = 2;

    loadGame(apples, aps, apc, ajc, atc, afc, isdiagon, ftf, apiec);

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

    sf::Texture upgbtnTexture;

    if (!upgbtnTexture.loadFromFile("assets/upg-button.png")) {
        return 1;
    }

    sf::Texture dlgTexture;

    if (!dlgTexture.loadFromFile("assets/dialog.png")) {
        return 1;
    }

    sf::Texture bgTexture;

    if (!bgTexture.loadFromFile("assets/background.png")) {
        return 1;
    }

    sf::Sprite bg(bgTexture);

    sf::Vector2u bgtextureSize = bgTexture.getSize();

    float bgscale = std::min(
        1920.f / bgtextureSize.x,
        1080.f / bgtextureSize.y
    );

    bg.setScale({bgscale, bgscale});

    //appel

    sf::Sprite apple(appleTexture);

    sf::Vector2u textureSize = appleTexture.getSize();

    float scale = std::min(
        200.f / textureSize.x,
        200.f / textureSize.y
    );

    apple.setScale({scale, scale});
    apple.setPosition({960.f, 720.f});
    apple.setOrigin({
        apple.getLocalBounds().size.x / 2.f,
        apple.getLocalBounds().size.y / 2.f - 80.f
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

    //dialogs/ach
    sf::Sprite dialog(dlgTexture);
    dialog.setPosition({960.f, 840.f});
    dialog.setOrigin({750.f, 100.f});

    sf::Text dlgText(font);
    dlgText.setCharacterSize(50);
    dlgText.setFillColor(sf::Color::Black);
    dlgText.setPosition({240.f, 810.f});

    //menu buttons

    sf::Sprite clickbtn1(upgbtnTexture);
    clickbtn1.setPosition({1630.f, 150.f});
    clickbtn1.setOrigin({180.f, 40.f});

    sf::Text cb1Text(font);
    cb1Text.setCharacterSize(32);
    cb1Text.setFillColor(sf::Color::Black);
    cb1Text.setPosition({1472.f, 132.f});

    sf::Sprite secbtn1(upgbtnTexture);
    secbtn1.setPosition({1630.f, 300.f});
    secbtn1.setOrigin({180.f, 40.f});

    sf::Text sb1Text(font);
    sb1Text.setCharacterSize(32);
    sb1Text.setFillColor(sf::Color::Black);
    sb1Text.setPosition({1472.f, 282.f});

    sf::Sprite secbtn2(upgbtnTexture);
    secbtn2.setPosition({1630.f, 450.f});
    secbtn2.setOrigin({180.f, 40.f});

    sf::Text sb2Text(font);
    sb2Text.setCharacterSize(32);
    sb2Text.setFillColor(sf::Color::Black);
    sb2Text.setPosition({1472.f, 432.f});

    sf::Sprite clickbtn2(upgbtnTexture);
    clickbtn2.setPosition({1630.f, 600.f});
    clickbtn2.setOrigin({180.f, 40.f});

    sf::Text cb2Text(font);
    cb2Text.setCharacterSize(32);
    cb2Text.setFillColor(sf::Color::Black);
    cb2Text.setPosition({1472.f, 582.f});


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

    appleText.setString("Apples: " + formatNumber(apples));
    std::ostringstream stream2;
    stream2 << std::fixed << std::setprecision(0) << apc;
    apcText.setString("Apples per click: " + stream2.str());
    std::ostringstream stream3;
    stream3 << std::fixed << std::setprecision(0) << aps;
    apsText.setString("Apples per second: " + stream3.str());
    std::ostringstream stream4;
    stream4 << std::fixed << std::setprecision(0) << ajc;
    cb1Text.setString("Apple Juice: " + stream4.str());
    std::ostringstream stream5;
    stream5 << std::fixed << std::setprecision(0) << atc;
    sb1Text.setString("Apple Tree: " + stream5.str());
    std::ostringstream stream6;
    stream6 << std::fixed << std::setprecision(0) << afc;
    sb2Text.setString("Apple Farm: " + stream6.str());
    std::ostringstream stream7;
    stream7 << std::fixed << std::setprecision(0) << apiec;
    cb2Text.setString("Apple Pie: " + stream7.str());
    dlgText.setString("btw the apple's name is Appel");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                saveGame(apples, aps, apc, ajc, atc, afc, isdiagon, ftf, apiec);
                window.close();
            }

            if (!isdiagon) {
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
                            appleText.setString("Apples: " + formatNumber(apples));
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
                            appleText.setString("Apples: " + formatNumber(apples));
                            std::ostringstream stream2;
                            stream2 << std::fixed << std::setprecision(0) << apc;
                            apcText.setString("Apples per click: " + stream2.str());
                            std::ostringstream stream3;
                            stream3 << std::fixed << std::setprecision(0) << ajc;
                            cb1Text.setString("Apple Juice: " + stream3.str());
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

                        if (secbtn1.getGlobalBounds().contains(mousePos) && apples >= atc) {
                            aps ++;
                            apples -= atc;
                            atc += 20;
                            appleText.setString("Apples: " + formatNumber(apples));
                            std::ostringstream stream2;
                            stream2 << std::fixed << std::setprecision(0) << atc;
                            sb1Text.setString("Apple Tree: " + stream2.str());
                            std::ostringstream stream3;
                            stream3 << std::fixed << std::setprecision(0) << aps;
                            apsText.setString("Apples per second: " + stream3.str());
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

                        if (secbtn2.getGlobalBounds().contains(mousePos) && apples >= afc) {
                            if (ftf == 2) {
                                ftf = 1;
                            } else if (ftf == 1) {
                                ftf = 0;
                            }
                            if (ftf == 1) {
                                isdiagon = true;
                            }
                            aps += 3;
                            apples -= afc;
                            afc += 350;
                            appleText.setString("Apples: " + formatNumber(apples));
                            std::ostringstream stream2;
                            stream2 << std::fixed << std::setprecision(0) << afc;
                            sb2Text.setString("Apple Farm: " + stream2.str());
                            std::ostringstream stream3;
                            stream3 << std::fixed << std::setprecision(0) << aps;
                            apsText.setString("Apples per second: " + stream3.str());
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

                        if (clickbtn2.getGlobalBounds().contains(mousePos) && apples >= apiec) {
                            apc += 5;
                            apples -= apiec;
                            apiec *= 2.5;
                            appleText.setString("Apples: " + formatNumber(apples));
                            std::ostringstream stream2;
                            stream2 << std::fixed << std::setprecision(0) << apc;
                            apcText.setString("Apples per click: " + stream2.str());
                            std::ostringstream stream7;
                            stream7 << std::fixed << std::setprecision(0) << apiec;
                            cb2Text.setString("Apple Pie: " + stream7.str());
                        }
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

                    if (dialog.getGlobalBounds().contains(mousePos) && isdiagon) {
                        isdiagon = false;
                    }
                }
            }
            float time = clock.getElapsedTime().asSeconds();
            float rotation = std::sin(time * 2.f) * 10.f;
            apple.setRotation(sf::degrees(rotation));
        }
        if (apsClock.getElapsedTime().asSeconds() >= 1.f) {
            apples += aps;
            appleText.setString("Apples: " + formatNumber(apples));
            apsClock.restart();
        }

        float time = clock.getElapsedTime().asSeconds();
        apple.setRotation(
            sf::degrees(std::sin(time * 2.f) * 8.f)
        );

        window.clear(sf::Color(200, 200, 200));

        window.draw(bg);
        window.draw(apple);
        window.draw(appleText);
        window.draw(apsText);
        window.draw(apcText);
        window.draw(menu);
        window.draw(clickbtn1);
        window.draw(cb1Text);
        window.draw(secbtn1);
        window.draw(sb1Text);
        window.draw(secbtn2);
        window.draw(sb2Text);
        window.draw(clickbtn2);
        window.draw(cb2Text);
        if (isdiagon) {
            window.draw(dialog);
            window.draw(dlgText);
        }


        window.display();
    }

    return 0;
}