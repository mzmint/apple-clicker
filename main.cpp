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

    if (number >= 1'000'000'000'000'000.0) {
        out << std::fixed << std::setprecision(1)
            << number / 1'000'000'000'000'000.0 << "Qa";
    } else if (number >= 1'000'000'000'000.0) {
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

void saveGame(double apples, double aps, double apc, double ajc, double atc, double afc, bool isdiagon, int ftf, int ftt, int ftfc, double apiec, double afcc, int jc, int tc, int fc, int pc, int fcc) {

    nlohmann::json save;

    save["apples"] = apples;
    save["aps"] = aps;
    save["apc"] = apc;
    save["ajc"] = ajc;
    save["atc"] = atc;
    save["afc"] = afc;
    save["isdiagon"] = isdiagon;
    save["ftf"] = ftf;
    save["ftt"] = ftt;
    save["ftfc"] = ftfc;
    save["apiec"] = apiec;
    save["afcc"] = afcc;
    save["jc"] = jc;
    save["tc"] = tc;
    save["fc"] = fc;
    save["pc"] = pc;
    save["fcc"] = fcc;

    std::ofstream file("save.json");

    file << save.dump(4);
}

void loadGame(double& apples, double& aps, double& apc, double& ajc, double& atc, double& afc, bool& isdiagon, int& ftf, int& ftt, int& ftfc, double& apiec, double& afcc, int& jc, int& tc, int& fc, int& pc, int& fcc)
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
    ftt = save.value("ftt", 2);
    ftfc = save.value("ftfc", 2);
    apiec = save.value("apiec", 1600);
    afcc = save.value("afcc", 24000);
    jc = save.value("jc", 0);
    tc = save.value("tc", 0);
    fc = save.value("fc", 0);
    pc = save.value("pc", 0);
    fcc = save.value("fcc", 0);
}

int main() {
    float volume = 50.f;

    sf::Music music;
    sf::Music ba;

    if (!music.openFromFile("assets/Appels.mp3")) {
        return 1;
    }

    if (!ba.openFromFile("assets/badapple.mp3")) {
        return 1;
    }

    music.setLooping(true);
    music.play();
    music.setVolume(volume);

    sf::Clock clock;
    sf::Clock apsClock;
    sf::Clock dlgClock;

    double apples = 0;
    double aps = 0;
    double apc = 1;
    double ajc = 10;
    double atc = 20;
    double afc = 400;
    double apiec = 1600;
    double afcc = 24000;
    bool isdiagon = false;
    bool issetton = false;
    int ftf = 2;
    int ftt = 2;
    int ftfc = 2;
    int jc = 0;
    int tc = 0;
    int fc = 0;
    int pc = 0;
    int fcc = 0;
    bool isba = false;

    loadGame(apples, aps, apc, ajc, atc, afc, isdiagon, ftf, ftt, ftfc, apiec, afcc, jc, tc, fc, pc, fcc);

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
    sf::Texture baTexture;

    if (!appleTexture.loadFromFile("assets/apple.png") ||
        !baTexture.loadFromFile("assets/badapple.png")) {
        return 1;
    }

    sf::Texture menuTexture;
    sf::Texture bamenuTexture;

    if (!menuTexture.loadFromFile("assets/menu.png") ||
        !bamenuTexture.loadFromFile("assets/bamenu.png")) {
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

    sf::Texture settingsTexture;

    if (!settingsTexture.loadFromFile("assets/settings.png")) {
        return 1;
    }

    sf::Texture settbtnTexture;

    if (!settbtnTexture.loadFromFile("assets/settbtn.png")) {
        return 1;
    }

    sf::Texture xbtnTexture;

    if (!xbtnTexture.loadFromFile("assets/xbtn.png")) {
        return 1;
    }

    sf::Texture tgonTexture;
    sf::Texture tgoffTexture;

    if (!tgonTexture.loadFromFile("assets/tgon.png") ||
    !tgoffTexture.loadFromFile("assets/tgoff.png")) {
        return 1;
    }

    sf::Texture bgTexture;
    sf::Texture bg2Texture;
    sf::Texture bg3Texture;
    sf::Texture bg4Texture;

    if (!bgTexture.loadFromFile("assets/background1.png") ||
    !bg2Texture.loadFromFile("assets/background2.png") ||
    !bg3Texture.loadFromFile("assets/background3.png") ||
    !bg4Texture.loadFromFile("assets/background4.png")) {
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

    //settings

    sf::Sprite settings(settingsTexture);
    settings.setOrigin({640.f, 360.f});
    settings.setPosition({960.f, 540.f});

    sf::Sprite settbtn(settbtnTexture);
    settbtn.setOrigin({40.f, 40.f});
    settbtn.setPosition({1800.f, 900.f});

    sf::Sprite settxbtn(xbtnTexture);
    settxbtn.setOrigin({40.f, 40.f});
    settxbtn.setPosition({1500.f, 280.f});

    sf::Text settText(font);
    settText.setCharacterSize(72);
    settText.setFillColor(sf::Color::Black);
    settText.setPosition({380.f, 240.f});
    settText.setString("Settings");

    sf::Text musText(font);
    musText.setCharacterSize(50);
    musText.setFillColor(sf::Color::Black);
    musText.setPosition({380.f, 400.f});
    musText.setString("Music:");

    sf::Text meText(font);
    meText.setCharacterSize(24);
    meText.setFillColor(sf::Color(128, 128, 128));
    meText.setPosition({1180.f, 820.f});
    meText.setString("Made by MzMint    v1.1.0");

    sf::Sprite mustgbtn(tgonTexture);
    mustgbtn.setOrigin({40.f, 40.f});
    mustgbtn.setPosition({640.f, 445.f});

    sf::Text baText(font);
    baText.setCharacterSize(50);
    baText.setFillColor(sf::Color::Black);
    baText.setPosition({380.f, 470.f});
    baText.setString("Bad Apple:");

    sf::Sprite batgbtn(tgoffTexture);
    batgbtn.setOrigin({40.f, 40.f});
    batgbtn.setPosition({760.f, 515.f});

    //dialogs/ach
    sf::Sprite dialog(dlgTexture);
    dialog.setPosition({960.f, 1080.f});
    dialog.setOrigin({750.f, 100.f});

    sf::Text dlgText(font);
    dlgText.setCharacterSize(50);
    dlgText.setFillColor(sf::Color::Black);
    dlgText.setPosition({240.f, 1050.f});

    //menu buttons

    sf::Sprite clickbtn1(upgbtnTexture);
    clickbtn1.setPosition({1630.f, 150.f});
    clickbtn1.setOrigin({210.f, 40.f});

    sf::Text cb1Text(font);
    cb1Text.setCharacterSize(30);
    cb1Text.setFillColor(sf::Color::Black);
    cb1Text.setPosition({1432.f, 122.f});

    sf::Text jcText(font);
    jcText.setCharacterSize(12);
    jcText.setFillColor(sf::Color(128, 128, 128));
    jcText.setPosition({1790.f, 165.f});

    sf::Sprite secbtn1(upgbtnTexture);
    secbtn1.setPosition({1630.f, 300.f});
    secbtn1.setOrigin({210.f, 40.f});

    sf::Text sb1Text(font);
    sb1Text.setCharacterSize(30);
    sb1Text.setFillColor(sf::Color::Black);
    sb1Text.setPosition({1432.f, 272.f});

    sf::Text tcText(font);
    tcText.setCharacterSize(12);
    tcText.setFillColor(sf::Color(128, 128, 128));
    tcText.setPosition({1790.f, 315.f});

    sf::Sprite secbtn2(upgbtnTexture);
    secbtn2.setPosition({1630.f, 450.f});
    secbtn2.setOrigin({210.f, 40.f});

    sf::Text sb2Text(font);
    sb2Text.setCharacterSize(30);
    sb2Text.setFillColor(sf::Color::Black);
    sb2Text.setPosition({1432.f, 422.f});

    sf::Text fcText(font);
    fcText.setCharacterSize(12);
    fcText.setFillColor(sf::Color(128, 128, 128));
    fcText.setPosition({1790.f, 465.f});

    sf::Sprite clickbtn2(upgbtnTexture);
    clickbtn2.setPosition({1630.f, 600.f});
    clickbtn2.setOrigin({210.f, 40.f});

    sf::Text cb2Text(font);
    cb2Text.setCharacterSize(30);
    cb2Text.setFillColor(sf::Color::Black);
    cb2Text.setPosition({1432.f, 572.f});

    sf::Text pcText(font);
    pcText.setCharacterSize(12);
    pcText.setFillColor(sf::Color(128, 128, 128));
    pcText.setPosition({1790.f, 615.f});

    sf::Sprite secbtn3(upgbtnTexture);
    secbtn3.setPosition({1630.f, 750.f});
    secbtn3.setOrigin({210.f, 40.f});

    sf::Text sb3Text(font);
    sb3Text.setCharacterSize(30);
    sb3Text.setFillColor(sf::Color::Black);
    sb3Text.setPosition({1432.f, 722.f});

    sf::Text fccText(font);
    fccText.setCharacterSize(12);
    fccText.setFillColor(sf::Color(128, 128, 128));
    fccText.setPosition({1790.f, 765.f});


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
    apcText.setString("Apples per click: " + formatNumber(apc));
    apsText.setString("Apples per second: " + formatNumber(aps));
    cb1Text.setString("Apple Juice: " + formatNumber(ajc));
    sb1Text.setString("Apple Tree: " + formatNumber(atc));
    sb2Text.setString("Apple Farm: " + formatNumber(afc));
    cb2Text.setString("Apple Pie: " + formatNumber(apiec));
    sb3Text.setString("Apple Factory: " + formatNumber(afcc));
    jcText.setString("x" + formatNumber(jc));
    tcText.setString("x" + formatNumber(tc));
    fcText.setString("x" + formatNumber(fc));
    pcText.setString("x" + formatNumber(pc));
    fccText.setString("x" + formatNumber(fcc));
    dlgText.setString("btw the apple's name is Appel");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                saveGame(apples, aps, apc, ajc, atc, afc, isdiagon, ftf, ftt, ftfc, apiec, afcc, jc, tc, fc, pc, fcc);
                window.close();
            }

            if (!isdiagon && !issetton) {
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
                            jc++;
                            appleText.setString("Apples: " + formatNumber(apples));
                            apcText.setString("Apples per click: " + formatNumber(apc));
                            cb1Text.setString("Apple Juice: " + formatNumber(ajc));
                            jcText.setString("x" + formatNumber(jc));
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
                            if (ftf == 2 && ftfc == 2) {
                                if (ftt == 2) {
                                    ftt = 1;
                                } else if (ftt == 1) {
                                    ftt = 0;
                                }
                                if (ftt == 1) {
                                    bg.setTexture(bg2Texture);
                                }
                            }
                            aps ++;
                            apples -= atc;
                            atc += 20;
                            tc++;
                            appleText.setString("Apples: " + formatNumber(apples));
                            sb1Text.setString("Apple Tree: " + formatNumber(atc));
                            apsText.setString("Apples per second: " + formatNumber(aps));
                            tcText.setString("x" + formatNumber(tc));
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
                            if (ftfc == 2) {
                                if (ftf == 2) {
                                    ftf = 1;
                                } else if (ftf == 1) {
                                    ftf = 0;
                                }
                                if (ftf == 1) {
                                    dlgClock.restart();
                                    isdiagon = true;
                                    bg.setTexture(bg3Texture);
                                }
                            }
                            aps += 3;
                            apples -= afc;
                            afc += 350;
                            fc++;
                            appleText.setString("Apples: " + formatNumber(apples));
                            sb2Text.setString("Apple Farm: " + formatNumber(afc));
                            apsText.setString("Apples per second: " + formatNumber(aps));
                            fcText.setString("x" + formatNumber(fc));
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
                            pc++;
                            appleText.setString("Apples: " + formatNumber(apples));
                            apcText.setString("Apples per click: " + formatNumber(apc));
                            cb2Text.setString("Apple Pie: " + formatNumber(apc));
                            pcText.setString("x" + formatNumber(pc));
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

                        if (settbtn.getGlobalBounds().contains(mousePos)) {
                            issetton = true;
                        }
                    }
                }
            }
            if (issetton) {
                if (const auto* mouse =
                            event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = {
                            static_cast<float>(mouse->position.x),
                            static_cast<float>(mouse->position.y)
                        };

                        if (settxbtn.getGlobalBounds().contains(mousePos)) {
                            issetton = false;
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

                        if (mustgbtn.getGlobalBounds().contains(mousePos)) {
                            if (volume == 50.f) {
                                volume = 0.f;
                                mustgbtn.setTexture(tgoffTexture);
                            } else {
                                volume = 50.f;
                                mustgbtn.setTexture(tgonTexture);
                            }
                            music.setVolume(volume);
                            ba.setVolume(volume);
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

                        if (batgbtn.getGlobalBounds().contains(mousePos)) {
                            if (isba) {
                                ba.stop();
                                music.setLooping(true);
                                music.play();
                                music.setVolume(volume);
                                isba = false;
                                apple.setTexture(appleTexture);
                                menu.setTexture(menuTexture);
                                batgbtn.setTexture(tgoffTexture);
                            } else {
                                ba.setLooping(true);
                                ba.play();
                                ba.setVolume(volume);
                                music.stop();
                                isba = true;
                                apple.setTexture(baTexture);
                                menu.setTexture(bamenuTexture);
                                batgbtn.setTexture(tgonTexture);
                            }
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
            if (const auto* mouse =
                        event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = {
                        static_cast<float>(mouse->position.x),
                        static_cast<float>(mouse->position.y)
                    };

                    if (secbtn3.getGlobalBounds().contains(mousePos) && apples >= afcc) {
                        if (ftfc == 2) {
                            ftfc = 1;
                        } else if (ftfc == 1) {
                            ftfc = 0;
                        }
                        if (ftfc == 1) {
                            bg.setTexture(bg4Texture);
                        }
                        aps += 20;
                        apples -= afcc;
                        afcc += 20000;
                        fcc++;
                        appleText.setString("Apples: " + formatNumber(apples));
                        sb3Text.setString("Apple Factory: " + formatNumber(afcc));
                        apsText.setString("Apples per second: " + formatNumber(aps));
                        fccText.setString("x" + formatNumber(fcc));
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

        window.clear(sf::Color::White);


        if (!isba) {
            window.draw(bg);
        }
        window.draw(apple);
        window.draw(appleText);
        window.draw(apsText);
        window.draw(apcText);
        window.draw(menu);
        window.draw(clickbtn1);
        window.draw(cb1Text);
        window.draw(jcText);
        window.draw(secbtn1);
        window.draw(sb1Text);
        window.draw(tcText);
        window.draw(secbtn2);
        window.draw(sb2Text);
        window.draw(fcText);
        window.draw(clickbtn2);
        window.draw(cb2Text);
        window.draw(pcText);
        window.draw(secbtn3);
        window.draw(sb3Text);
        window.draw(fccText);
        window.draw(settbtn);
        if (isdiagon) {
            float ddt = dlgClock.restart().asSeconds();
            float speed = 1200.f;

            sf::Vector2f target = {960.f, 800.f};
            sf::Vector2f pos = dialog.getPosition();

            if (pos.y > target.y) {
                float movement = speed * ddt;
                if (pos.y - movement < target.y)
                    movement = pos.y - target.y;
                dialog.move({0.f, -movement});
                dlgText.move({0.f, -movement});
            }
            window.draw(dialog);
            window.draw(dlgText);
        }
        if (issetton) {
            window.draw(settings);
            window.draw(settxbtn);
            window.draw(settText);
            window.draw(musText);
            window.draw(mustgbtn);
            window.draw(meText);
            window.draw(baText);
            window.draw(batgbtn);
        }

        window.display();
    }

    return 0;
}