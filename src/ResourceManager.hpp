#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

class ResourceManager {
private:
    ResourceManager(const std::string& base_dir) : base_dir_(base_dir) {}

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

public:
    static void Initialize(const std::string& base_dir = std::string());

    static std::shared_ptr<ResourceManager> GetInstance();

    sf::Texture* LoadTexture(const std::string& name);
    sf::Font* LoadFont(const std::string& name);

private:
    static std::shared_ptr<ResourceManager> instance_;

private:
    std::string base_dir_;
    std::unordered_map<std::string, sf::Texture> textures_;
    std::unordered_map<std::string, sf::Font> fonts_;
};
