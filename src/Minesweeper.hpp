#pragma once

#include <cstdint>

#include <list>
#include <random>

#include <SFML/System.hpp>

class Minesweeper {
public:
    struct Tile {
        Tile() : bombs_around(0), has_bomb(0), has_flag(0), is_discovered(0) {}

        uint8_t bombs_around;
        bool has_bomb;
        bool has_flag;
        bool is_discovered;
    };

    using Adjacents = std::list<std::pair<sf::Vector2<size_t>, Tile*>>;

    enum class GameState {
        WON,
        LOST,
        INGAME,
    };

public:
    Minesweeper(size_t width, size_t height, size_t num_bombs);

    Tile& GetTile(size_t x, size_t y);

    size_t GetWidth() const {
        return width_;
    }

    size_t GetHeight() const {
        return height_;
    }

    Adjacents GetAdjacentTiles(size_t x, size_t y);

    void Dig(size_t x, size_t y);

    void ToggleFlag(size_t x, size_t y);

    GameState GetGameState() const;

    void PrintBombs();

private:
    size_t width_;
    size_t height_;
    size_t num_mines_;
    std::vector<Tile> tiles_;
};
