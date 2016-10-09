#pragma once

#include "Minesweeper.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

enum class SpriteID : uint8_t {
    BLOCK_BACKGROUND = 0,
    BLOCK_NORMAL,
    BOMB,
    BOMB_RED,
    FLAG_GREEN,
    FLAG_ORANGE,
    NUMBER_1,
    NUMBER_2,
    NUMBER_3,
    NUMBER_4,
    NUMBER_5,
    NUMBER_6,
    NUMBER_7,
    NUMBER_8,
};

struct SpriteIDHash {
    uint8_t operator()(SpriteID t) const {
        return static_cast<uint8_t>(t);
    }
};

class GameMap {
public:
    GameMap(size_t map_width, size_t map_height, size_t mines_width,
            size_t mines_height, size_t num_mines);

    sf::Vector2f GetSize() const;
    void SetSize(const sf::Vector2f& size);

    const sf::Vector2f& GetPosition() const;
    void SetPosition(const sf::Vector2f& pos);

    const sf::Vector2f& GetTileSize() const;

    void Dig(const sf::Vector2f& click_pos);

    void ToggleFlag(const sf::Vector2f& click_pos);

    size_t NumFlagsPlaced() const;

    void Update();

    void Draw(sf::RenderWindow& window);

private:
    sf::Vector2<size_t> GetTilePos(const sf::Vector2f& pos);

    sf::Sprite* GetBackgroundSprite(size_t x, size_t y);
    sf::Sprite* GetItemSprite(size_t x, size_t y);

private:
    sf::Vector2f position_;
    sf::Vector2f tile_size_;

    Minesweeper mines_;
    Minesweeper::GameState current_game_state_;
    std::unordered_map<SpriteID, sf::Sprite, SpriteIDHash> sprites_;
};
