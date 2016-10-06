#include "GameMap.hpp"

#include <cmath>

#define TEXTURE_SIZE 70

GameMap::GameMap(size_t map_width, size_t map_height, size_t mines_width,
                 size_t mines_height, size_t num_mines)
      : position_(0, 0),
        tile_size_(0, 0),
        mines_(mines_width, mines_height, num_mines),
        current_game_state_(Minesweeper::GameState::INGAME) {
    auto res = ResourceManager::GetInstance();

    // Load the textures
    auto block_bg_tex = res->LoadTexture("assets/tiles/block_background.png");
    auto block_normal_tex = res->LoadTexture("assets/tiles/block_normal.png");
    auto bomb_tex = res->LoadTexture("assets/tiles/bomb.png");
    auto bomb_red_tex = res->LoadTexture("assets/tiles/bomb_red.png");
    auto flag_green_tex = res->LoadTexture("assets/tiles/flag_green.png");
    auto flag_orange_tex = res->LoadTexture("assets/tiles/flag_orange.png");
    auto number_1_tex = res->LoadTexture("assets/tiles/number_1.png");
    auto number_2_tex = res->LoadTexture("assets/tiles/number_2.png");
    auto number_3_tex = res->LoadTexture("assets/tiles/number_3.png");
    auto number_4_tex = res->LoadTexture("assets/tiles/number_4.png");
    auto number_5_tex = res->LoadTexture("assets/tiles/number_5.png");
    auto number_6_tex = res->LoadTexture("assets/tiles/number_6.png");
    auto number_7_tex = res->LoadTexture("assets/tiles/number_7.png");
    auto number_8_tex = res->LoadTexture("assets/tiles/number_8.png");

    // Load the fonts
    res->LoadFont("assets/font/Ubuntu-B.ttf");

    // Register each sprite
    sprites_[SpriteID::BLOCK_BACKGROUND] = sf::Sprite(*block_bg_tex);
    sprites_[SpriteID::BLOCK_NORMAL] = sf::Sprite(*block_normal_tex);
    sprites_[SpriteID::BOMB] = sf::Sprite(*bomb_tex);
    sprites_[SpriteID::BOMB_RED] = sf::Sprite(*bomb_red_tex);
    sprites_[SpriteID::FLAG_GREEN] = sf::Sprite(*flag_green_tex);
    sprites_[SpriteID::FLAG_ORANGE] = sf::Sprite(*flag_orange_tex);
    sprites_[SpriteID::NUMBER_1] = sf::Sprite(*number_1_tex);
    sprites_[SpriteID::NUMBER_2] = sf::Sprite(*number_2_tex);
    sprites_[SpriteID::NUMBER_3] = sf::Sprite(*number_3_tex);
    sprites_[SpriteID::NUMBER_4] = sf::Sprite(*number_4_tex);
    sprites_[SpriteID::NUMBER_5] = sf::Sprite(*number_5_tex);
    sprites_[SpriteID::NUMBER_6] = sf::Sprite(*number_6_tex);
    sprites_[SpriteID::NUMBER_7] = sf::Sprite(*number_7_tex);
    sprites_[SpriteID::NUMBER_8] = sf::Sprite(*number_8_tex);

    // Change the map size
    SetSize({float(map_width), float(map_height)});
}

sf::Vector2f GameMap::GetSize() const {
    return {tile_size_.x * mines_.GetWidth(),
            tile_size_.y * mines_.GetHeight()};
}

void GameMap::SetSize(const sf::Vector2f& size) {
    tile_size_ = {size.x / mines_.GetWidth(), size.y / mines_.GetHeight()};
    for (auto& pair : sprites_) {
        pair.second.setScale(tile_size_ / float(TEXTURE_SIZE));
    }
}

const sf::Vector2f& GameMap::GetPosition() const {
    return position_;
};

void GameMap::SetPosition(const sf::Vector2f& pos) {
    position_ = pos;
};

const sf::Vector2f& GameMap::GetTileSize() const {
    return tile_size_;
}

void GameMap::Dig(const sf::Vector2f& click_pos) {
    if (current_game_state_ != Minesweeper::GameState::INGAME) return;
    sf::Vector2<size_t> tile_pos = GetTilePos(click_pos);
    if (tile_pos.x < mines_.GetWidth() && tile_pos.y < mines_.GetHeight())
        mines_.Dig(tile_pos.x, tile_pos.y);
}

void GameMap::ToggleFlag(const sf::Vector2f& click_pos) {
    if (current_game_state_ != Minesweeper::GameState::INGAME) return;
    sf::Vector2<size_t> tile_pos = GetTilePos(click_pos);
    if (tile_pos.x < mines_.GetWidth() && tile_pos.y < mines_.GetHeight())
        mines_.ToggleFlag(tile_pos.x, tile_pos.y);
}

void GameMap::Update() {
    current_game_state_ = mines_.GetGameState();
}

void GameMap::Draw(sf::RenderWindow& window) {
    for (size_t j = 0; j < mines_.GetHeight(); j++) {
        for (size_t i = 0; i < mines_.GetWidth(); i++) {
            sf::Sprite* bg_sprite = GetBackgroundSprite(i, j);
            sf::Sprite* icon_sprite = GetItemSprite(i, j);
            sf::Vector2f tile_pos(GetPosition().x + i * GetTileSize().x,
                                  GetPosition().y + j * GetTileSize().y);
            if (bg_sprite) {
                bg_sprite->setPosition(tile_pos);
                window.draw(*bg_sprite);
            }
            if (icon_sprite) {
                icon_sprite->setPosition(tile_pos);
                window.draw(*icon_sprite);
            }
        }
    }

    // Draw the GameState
    if (current_game_state_ != Minesweeper::GameState::INGAME) {
        std::string text_str;
        if (current_game_state_ == Minesweeper::GameState::WON) {
            text_str = "Ganaste";
        } else if (current_game_state_ == Minesweeper::GameState::LOST) {
            text_str = "Perdiste";
        }

        auto res = ResourceManager::GetInstance();
        auto default_font = res->LoadFont("assets/font/Ubuntu-B.ttf");

        sf::Vector2f map_size(GetSize());
        sf::Vector2f map_center(GetPosition() + map_size / 2.f);

        // Draw the background
        sf::Vector2f block_size(map_size.x, map_size.y / 3.f);
        sf::RectangleShape block(block_size);
        block.setOrigin(block_size / 2.f);
        block.setPosition(map_center);
        block.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(block);

        // Draw the state of the game
        sf::Text text(text_str, *default_font, 50);
        text.setStyle(sf::Text::Bold);
#if (SFML_VERSION_MINOR >= 4)
        text.setFillColor(sf::Color::White);
#else
        text.setColor(sf::Color::White);
#endif
        auto rect = text.getGlobalBounds();
        text.setOrigin(rect.width / 2.f, rect.height / 2.f);
        text.setPosition(map_center);
        window.draw(text);
    }
}

sf::Vector2<size_t> GameMap::GetTilePos(const sf::Vector2f& pos) {
    return {static_cast<size_t>((pos.x - GetPosition().x) / GetTileSize().x),
            static_cast<size_t>((pos.y - GetPosition().y) / GetTileSize().y)};
}

sf::Sprite* GameMap::GetBackgroundSprite(size_t x, size_t y) {
    Minesweeper::Tile& t = mines_.GetTile(x, y);

    if (!t.is_discovered) {
        return &sprites_[SpriteID::BLOCK_NORMAL];
    } else {
        return &sprites_[SpriteID::BLOCK_BACKGROUND];
    }
    return nullptr;
}

sf::Sprite* GameMap::GetItemSprite(size_t x, size_t y) {
    Minesweeper::Tile& t = mines_.GetTile(x, y);

    sf::Sprite* result = nullptr;

    if (t.is_discovered) {
        if (t.bombs_around == 1) {
            result = &sprites_[SpriteID::NUMBER_1];
        } else if (t.bombs_around == 2) {
            result = &sprites_[SpriteID::NUMBER_2];
        } else if (t.bombs_around == 3) {
            result = &sprites_[SpriteID::NUMBER_3];
        } else if (t.bombs_around == 4) {
            result = &sprites_[SpriteID::NUMBER_4];
        } else if (t.bombs_around == 5) {
            result = &sprites_[SpriteID::NUMBER_5];
        } else if (t.bombs_around == 6) {
            result = &sprites_[SpriteID::NUMBER_6];
        } else if (t.bombs_around == 7) {
            result = &sprites_[SpriteID::NUMBER_7];
        } else if (t.bombs_around == 8) {
            result = &sprites_[SpriteID::NUMBER_8];
        }
    } else {
        if (t.has_flag) {
            result = &sprites_[SpriteID::FLAG_ORANGE];
        }
    }

    if (current_game_state_ != Minesweeper::GameState::INGAME) {
        if (t.has_bomb) {
            if (t.is_discovered) {
                result = &sprites_[SpriteID::BOMB_RED];
            } else if (current_game_state_ == Minesweeper::GameState::WON) {
                result = &sprites_[SpriteID::FLAG_GREEN];
            } else if (current_game_state_ == Minesweeper::GameState::LOST) {
                if (t.has_flag) {
                    result = &sprites_[SpriteID::FLAG_GREEN];
                } else {
                    result = &sprites_[SpriteID::BOMB];
                }
            }
        }
    }

    return result;
}
