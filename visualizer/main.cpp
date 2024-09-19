#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include <vector>

#include "grid_pathfinding.h"

void setupMainMenu(tgui::Gui& gui, std::vector<std::string>& names, sf::RenderWindow& main_menu_window);

int main() {
    sf::RenderWindow main_menu_window(sf::VideoMode(800, 600), "Visualizer");

    tgui::Gui gui(main_menu_window);

    std::vector<std::string> names = {"Grid Pathfinding"};

    setupMainMenu(gui, names, main_menu_window);

    while (main_menu_window.isOpen()) {
        sf::Event event;
        while (main_menu_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                main_menu_window.close();
            }

            gui.handleEvent(event);
        }

        main_menu_window.clear();
        gui.draw();
        main_menu_window.display();
    }

    return 0;
}

void setupMainMenu(tgui::Gui& gui, std::vector<std::string>& names, sf::RenderWindow& main_menu_window) {
    float height = static_cast<float>(gui.getTarget()->getSize().y);

    auto titleText = tgui::Label::create();
    titleText->setText("Choose an option:");
    titleText->setTextSize(height * 0.06f);
    titleText->setPosition("(&.width - width) / 2", height * 0.1f);
    titleText->getRenderer()->setTextColor(sf::Color::White);
    gui.add(titleText);

    float yPos = 200.f;

    for (const auto& name : names) {
        auto button = tgui::Button::create();
        button->setSize(250, height * 0.1f);
        button->setPosition("(&.width - width) / 2", yPos);
        button->getRenderer()->setBackgroundColor(sf::Color::White);
        button->getRenderer()->setBorderColor(sf::Color(105, 105, 105));
        button->getRenderer()->setBorders({3.f, 3.f, 3.f, 3.f});
        button->setText(name);
        button->setTextSize(height * 0.03f);
        button->getRenderer()->setTextColor(sf::Color::Black);
        button->onPress([name, &main_menu_window]() {
            std::cout << "You clicked on " << name << "!" << std::endl;
            if (name == "Grid Pathfinding") {
                main_menu_window.close();
                GridPathfindingVisualizer pathfinding_scenario;
                pathfinding_scenario.run();
            }
        });
        gui.add(button);

        yPos += (height * 0.1f) + 20.f;
    }
}