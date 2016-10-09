#include <iostream>
#include <list>
#include <utility>

#include "Minesweeper.hpp"

Minesweeper::Minesweeper(size_t width, size_t height, size_t num_mines)
      : width_(width),
        height_(height),
        num_mines_(num_mines),
        tiles_(width * height) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis_pos(0, height * width - 1);

    for (size_t i = 0; i < num_mines; i++) {
        size_t pos = dis_pos(gen);
        if (!tiles_[pos].has_bomb) {
            tiles_[pos].has_bomb = true;
            auto adjacents = GetAdjacentTiles(pos % width, pos / width);
            for (auto adj : adjacents) {
                adj.second->bombs_around++;
            }
        } else {
            i--;
        }
    }
}

Minesweeper::Tile& Minesweeper::GetTile(size_t x, size_t y) {
    return tiles_[y * width_ + x];
}

Minesweeper::Adjacents Minesweeper::GetAdjacentTiles(size_t x, size_t y) {
    Minesweeper::Adjacents result;
    if (x >= width_ || y >= height_) return result;

    sf::Vector2<size_t> adjacent_positions[8] = {
        {x + 1, y}, {x + 1, y + 1}, {x, y + 1}, {x - 1, y + 1},
        {x - 1, y}, {x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1}};

    for (size_t i = 0; i < 8; i++) {
        sf::Vector2<size_t>& pos = adjacent_positions[i];
        if (pos.x < width_ && pos.y < height_) {
            result.push_back({{pos.x, pos.y}, &GetTile(pos.x, pos.y)});
        }
    }

    return result;
}

void Minesweeper::Dig(size_t x, size_t y) {
    sf::Vector2<size_t> pos;
    Minesweeper::Tile* tile = &GetTile(x, y);

    if (tile->has_flag) {
        return;
    };

    if (tile->has_bomb) {
        tile->is_discovered = true;
        return;
    };

    Minesweeper::Adjacents queue;
    queue.push_back({{x, y}, tile});

    while (!queue.empty()) {
        pos = queue.front().first;
        tile = queue.front().second;
        queue.pop_front();
        if (tile->is_discovered) continue;
        tile->is_discovered = true;
        if (tile->has_flag) tile->has_flag = false;
        if (tile->bombs_around == 0 && !tile->has_bomb) {
            auto more = GetAdjacentTiles(pos.x, pos.y);
            queue.splice(queue.end(), more);
        }
    }
};

void Minesweeper::ToggleFlag(size_t x, size_t y) {
    Minesweeper::Tile* tile = &GetTile(x, y);
    tile->has_flag = !tile->has_flag;
};

Minesweeper::GameState Minesweeper::GetGameState() const {
    size_t num_discovered = 0;

    for (auto& tile : tiles_) {
        if (tile.has_bomb) {
            // Check if any mine has been digged
            if (tile.is_discovered) {
                return GameState::LOST;
            }
        } else if (tile.is_discovered) {
            num_discovered++;
        }
    }

    // Return won if al the non-mine Tiles has been discovered
    if (num_discovered == (GetWidth() * GetHeight() - num_mines_)) {
        return GameState::WON;
    } else {
        return GameState::INGAME;
    }
}

size_t Minesweeper::NumFlagsPlaced() const {
    size_t num_flags = 0;

    for (auto& tile : tiles_) {
        if (tile.has_flag && !tile.is_discovered) {
            num_flags++;
        }
    }

    return num_flags;
}

void Minesweeper::PrintBombs() {
    for (size_t j = 0; j < height_; j++) {
        for (size_t i = 0; i < width_; i++) {
            if (i) std::cout << " ";

            Minesweeper::Tile& t = GetTile(i, j);
            if (!t.is_discovered) {
                std::cout << "X";
            } else if (t.has_bomb) {
                std::cout << 'B';
            } else if (t.has_flag) {
                std::cout << 'F';
            } else {
                std::cout << static_cast<int>(t.bombs_around);
            }
        }
        std::cout << std::endl;
    }
}
