#include <chrono>
#include <sstream>
#include <iomanip>

#include "GameMap.hpp"

#include <SFML/System.hpp>

int main(int, char* argv[]) {
    ResourceManager::Initialize(argv[0]);
    auto res = ResourceManager::GetInstance();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Buscaminas",
                            sf::Style::Titlebar | sf::Style::Close);

    // Enable V-Sync
    window.setVerticalSyncEnabled(true);

    // Game configuration
    size_t board_size = 8;
    size_t num_mines = 10;

    // Load a map to display
    GameMap map(600, 600, board_size, board_size, num_mines);

    // Create the score and timer items
    auto default_font = res->LoadFont("assets/font/Ubuntu-B.ttf");
    auto flag_tex = res->LoadTexture("assets/tiles/flag_orange.png");
    sf::Sprite flag_spr(*flag_tex);
    flag_spr.setPosition({700 - flag_spr.getGlobalBounds().width / 2, 0});
    sf::Text score("0/0", *default_font);

    // Create the timer
    auto start_time = std::chrono::steady_clock::now();
    sf::Text timer("00:00", *default_font);

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
                        map.Dig(pos);
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        map.ToggleFlag(pos);
                    }
                    map.Update();
                    break;
                }
                default: { break; }
            }
        }
        // Clear screen
        window.clear(sf::Color(255, 183, 112));

        // Draw the map
        map.Draw(window);

        // Draw the score
        score.setString(std::to_string(map.NumFlagsPlaced()) + '/' +
                        std::to_string(num_mines));
        score.setPosition({700 - score.getGlobalBounds().width / 2,
                           flag_spr.getGlobalBounds().height});
        window.draw(flag_spr);
        window.draw(score);

        // Draw the timer
        auto current_time = std::chrono::steady_clock::now();
        auto ticks = std::chrono::duration_cast<std::chrono::duration<size_t>>(
                         current_time - start_time)
                         .count();
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(2) << (ticks / 60) << ':'
               << std::setw(2) << (ticks % 60);
        timer.setString(stream.str());
        timer.setPosition({700 - timer.getGlobalBounds().width / 2, 200});
        window.draw(timer);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
