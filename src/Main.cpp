#include <iostream>

#include "Map.hpp"

#include <SFML/System.hpp>

int main(int, char* argv[]) {
    ResourceManager::Initialize(argv[0]);
    // auto resources = ResourceManager::GetInstance();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML window",
                            sf::Style::Titlebar | sf::Style::Close);

    // Load a map to display
    MineMap m(600, 600, 8, 8, 10);

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    sf::Vector2f pos(static_cast<float>(event.mouseButton.x),
                                     static_cast<float>(event.mouseButton.y));
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        m.Dig(pos);
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        m.ToggleFlag(pos);
                    }
                    m.Update();
                    break;
                }
                default: {}
            }
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        m.Draw(window);
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
